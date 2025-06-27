// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_LockOn_Player.h"

#include "TwoMinDebugHelper.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


void UTwoMinGA_LockOn_Player::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	DebugTwoMin::Print(TEXT("Lock On Ability Activated"), FColor::Green, 1);
	LockOnTarget = FindLockOnTarget();
	if (!LockOnTarget)
	{
		CustomCancelAbility();
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_LockOn_Player::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	DebugTwoMin::Print(TEXT("Lock On Ability End"), FColor::Red, 3);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AActor* UTwoMinGA_LockOn_Player::FindLockOnTarget()
{
	CheckSphereOverTargetGroup();

	if (LockOnTargetGroup.IsEmpty())
	{
		DebugTwoMin::Print(TEXT("Lock On Target Null"), FColor::Red, 2);
		return nullptr;
	}

	FString TargetName = FString::Printf(TEXT("Lock On Target Name: %s"),
		*LockOnTargetGroup[0]->GetActorNameOrLabel());
	DebugTwoMin::Print(TargetName, FColor::Green, 2);

	return LockOnTargetGroup[0];
}

void UTwoMinGA_LockOn_Player::CheckSphereOverTargetGroup()
{
	LockOnTargetGroup.Empty();

	const FVector Center = GetAvatarActorFromActorInfo()->GetActorLocation();
	const TArray<AActor*> IgnoreActors;

	if (bIsDebugLockOnRange)
	{
		DrawDebugSphere(GetWorld(), Center, LockOnDistance, 32, FColor::Yellow, false,
			2.0f, 0, 2.0f);	
	}
	
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Center,
		LockOnDistance,
		ObjectTypes,
		ATwoMinEnemyCharacter::StaticClass(),
		IgnoreActors,
		LockOnTargetGroup
	);
}
