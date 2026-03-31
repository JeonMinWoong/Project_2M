// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_ThrowBase.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Controller/TwoMinPlayerController.h"
#include "Item/Weapon/TwoMinWeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UTwoMinGA_ThrowBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	if (!ThrowAnimMontage)
	{
		CustomCancelAbility();
		return;
	}
	
	PlayToAnimMontage(ThrowAnimMontage);
	CachedProjectileMesh();
	
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, SpawnProjectileEventTag, nullptr, true, true);

	Task->EventReceived.AddDynamic(this, &ThisClass::OnAttackGameplayEventReceivedByRange);
	Task->ReadyForActivation();
	
	UTwoMinFunctionLibrary::AddGameplayTagToActor(GetAvatarActorFromActorInfo(), TwoMinGameplayTag::Player_State_ThrownWeapon);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_ThrowBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

TSubclassOf<UGameplayEffect> UTwoMinGA_ThrowBase::GetAttackGameplayEffectClass() const
{
	return AttackGameplayEffectClass;
}

void UTwoMinGA_ThrowBase::OnAttackGameplayEventReceivedByRange(FGameplayEventData Payload)
{
	Super::OnAttackGameplayEventReceivedByRange(Payload);
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;
	
	PlayerCharacter->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()[1]->EnableMesh(false);
}

void UTwoMinGA_ThrowBase::CachedProjectileMesh()
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;
	
	NewStaticMesh = PlayerCharacter->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()[1]->GetWeaponMesh()->GetStaticMesh();
}

FRotator UTwoMinGA_ThrowBase::GetThrowRotator()
{
	ATwoMinPlayerController* PlayerController = Cast<ATwoMinPlayerController>(CurrentActorInfo->PlayerController);
	if (!PlayerController) return FRotator::ZeroRotator;
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return FRotator::ZeroRotator;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	FVector2D ScreenCenter(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	
	FVector WorldLoc, WorldDir;
	if (UGameplayStatics::DeprojectScreenToWorld(PlayerController, ScreenCenter, WorldLoc, WorldDir))
	{
		FHitResult Hit;
		FVector TraceStart = WorldLoc;
		FVector TraceEnd = WorldLoc + (WorldDir * 10000.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(PlayerCharacter);

		FVector TargetPoint;
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
		{
			TargetPoint = Hit.ImpactPoint;
		}
		else
		{
			TargetPoint = TraceEnd;
		}
		
		FVector MuzzleLocation = PlayerCharacter->GetMesh()->GetSocketLocation(TEXT("ThrowSocket"));
		FRotator LaunchRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TargetPoint);
		return LaunchRotation;	
	}
	
	return FRotator::ZeroRotator;
}

void UTwoMinGA_ThrowBase::DestroyProjectile()
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;
	
	PlayerCharacter->GetCombatComponent()->GetCharacterCurrentEquippedWeapon()[1]->EnableMesh(true);
	UTwoMinFunctionLibrary::RemoveGameplayTagToActor(GetAvatarActorFromActorInfo(), TwoMinGameplayTag::Player_State_ThrownWeapon);
}
