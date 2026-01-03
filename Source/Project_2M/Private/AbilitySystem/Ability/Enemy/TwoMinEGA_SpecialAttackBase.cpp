// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinEGA_SpecialAttackBase.h"

#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"

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
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, OnHitEventTag, nullptr, false, true);

	Task->EventReceived.AddDynamic(this, &ThisClass::OnAttackGameplayEventReceivedByMelee);
	Task->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* ResetTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, TwoMinGameplayTag::Shared_Event_ResetAttackCount, nullptr, false,
	true);

	ResetTask->EventReceived.AddDynamic(this, &ThisClass::OnResetAttackCountGameplayEffectReceive);
	ResetTask->ReadyForActivation();
	
	if (HitCollisionMap.IsEmpty() == false)
	{
		WaitGameplayEvent(HitCollisionEventTag, true);	
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinEGA_SpecialAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
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

void UTwoMinEGA_SpecialAttackBase::CustomEventReceived(FGameplayEventData Payload)
{
	Super::CustomEventReceived(Payload);
	
	if (HitCollisionMap.Contains(CurComboCount) == false) return;
	
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo());
	if (!EnemyCharacter) return;
	
	EnableHitCollision(EnemyCharacter);
}