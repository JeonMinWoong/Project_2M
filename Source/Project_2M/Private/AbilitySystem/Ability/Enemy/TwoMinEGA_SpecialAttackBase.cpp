// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinEGA_SpecialAttackBase.h"

#include "NiagaraFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Ability/Task/TwoMinAT_TeleportTaskBase.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Item/HitBox/HitCollisionBase.h"

void UTwoMinEGA_SpecialAttackBase::AddComboCount()
{
	CurComboCount = FMath::Clamp(CurComboCount + 1, 1, MaxComboCount);
	if (ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo()))
	{
		EnemyCharacter->GetCombatComponent()->ClearOverlappingActors();
	}
}

void UTwoMinEGA_SpecialAttackBase::ResetComboCount()
{
	CurComboCount = 1;
}

void UTwoMinEGA_SpecialAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (AttackMontage == nullptr)
	{
		CustomCancelAbility();
		return;
	}

	PlayToAnimMontage(AttackMontage);
	
	UAbilityTask_WaitGameplayEvent* ResetTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, TwoMinGameplayTag::Shared_Event_ResetAttackCount, nullptr, 
		false,true);

	ResetTask->EventReceived.AddDynamic(this, &ThisClass::OnResetAttackCountGameplayEffectReceive);
	ResetTask->ReadyForActivation();
	
	if (OnHitEventTag.IsValid())
	{
		UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, OnHitEventTag, nullptr, false, true);

		Task->EventReceived.AddDynamic(this, &ThisClass::OnAttackGameplayEventReceivedByMelee);
		Task->ReadyForActivation();	
	}
	
	if (OnLocationEventTag.IsValid())
	{
		UAbilityTask_WaitGameplayEvent* LocationTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, OnLocationEventTag, nullptr, false, true);
	
		LocationTask->EventReceived.AddDynamic(this, &ThisClass::OnAttackGameplayEventReceivedByLocation);
		LocationTask->ReadyForActivation();	
	}
	
	if (HitCollisionMap.IsEmpty() == false)
	{
		WaitGameplayEvent(HitCollisionEventTag, false);	
	}
	
	if (TeleportTaskClass)
	{
		UTwoMinAT_TeleportTaskBase* TeleportTask = 
			UTwoMinAT_TeleportTaskBase::CreateTickTask(this, TeleportData);
		
		TeleportTask->FOnStartTeleport.AddUniqueDynamic(this, &ThisClass::OnStartTeleport);
		TeleportTask->FOnFinishTeleport.AddUniqueDynamic(this, &ThisClass::OnFinishTeleport);
		TeleportTask->ReadyForActivation();
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinEGA_SpecialAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ATwoMinEnemyCharacter* Enemy = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo()))
	{
		Enemy->GetCombatComponent()->ClearOverlappingActors();
	}
	
	ResetComboCount();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinEGA_SpecialAttackBase::OnResetAttackCountGameplayEffectReceive(FGameplayEventData Payload)
{
	ResetComboCount();
}

TSubclassOf<UGameplayEffect> UTwoMinEGA_SpecialAttackBase::GetAttackGameplayEffectClass() const
{
	return AttackGameplayEffectClass;
}

void UTwoMinEGA_SpecialAttackBase::OnAttackGameplayEventReceivedByLocation(FGameplayEventData Payload)
{
	AActor* Target = const_cast<AActor*>(Payload.Target.Get());
	if (!Target) return;
	
	FVector TargetLocation = Target->GetActorLocation();
	TargetLocation.Z = 0;
	
	CachedTargetLocation = TargetLocation;
	if (!CachedLocationEffect) return;
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		CachedLocationEffect,
		CachedTargetLocation,
		FRotator::ZeroRotator
	);
}

void UTwoMinEGA_SpecialAttackBase::CustomEventReceived(FGameplayEventData Payload)
{
	Super::CustomEventReceived(Payload);
	
	if (HitCollisionMap.Contains(CurComboCount) == false) return;
	
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo());
	if (!EnemyCharacter) return;
	
	AHitCollisionBase* HitCollisionClass = HitCollisionMap[CurComboCount]->GetDefaultObject<AHitCollisionBase>();
	if (!HitCollisionClass) return;
	
	FVector TargetLocation = FVector::ZeroVector;
	EHitCollisionSpawnType SpawnType = HitCollisionClass->GetHitCollisionSpawnType();
	if (SpawnType == EHitCollisionSpawnType::TargetLocation)
	{
		TargetLocation = CachedTargetLocation;
	}
	
	EnableHitCollision(EnemyCharacter, TargetLocation);
}
