// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinEGA_AttackBase.h"

#include "AbilitySystemComponent.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/TwoMinEnemyDummy.h"
#include "Compnents/Combat/BaseCombatComponent.h"

void UTwoMinEGA_AttackBase::AddComboCount()
{
	CurComboCount = FMath::Clamp(CurComboCount + 1, 1, MaxComboCount);
	if (ATwoMinEnemyCharacter* Enemy = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo()))
	{
		Enemy->GetCombatComponent()->ClearOverlappingActors();
	}
}

void UTwoMinEGA_AttackBase::ResetComboCount()
{
	CurComboCount = 1;
}

void UTwoMinEGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	if (AttackMontage == nullptr)
	{
		CustomCancelAbility();
		return;
	}
	
	if (ATwoMinEnemyDummy* Dummy = Cast<ATwoMinEnemyDummy>(GetAvatarActorFromActorInfo()))
	{
		StartLocation = Dummy->GetActorLocation();
	}

	PlayToAnimMontage(AttackMontage);

	switch (AttackRangeType)
	{
	case EAttackRangeType::Melee:
		{
			UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, OnHitEventTag, nullptr, false, true);

			Task->EventReceived.AddDynamic(this, &ThisClass::OnAttackGameplayEventReceivedByMelee);
			Task->ReadyForActivation();
		}
		break;
	case EAttackRangeType::Range:
		{
			UAbilityTask_WaitGameplayEvent* TargetTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
				this, OnTargetEventTag, nullptr, false, true);

			TargetTask->EventReceived.AddDynamic(this, &ThisClass::OnAbilityGameplayEventReceivedByTarget);
			TargetTask->ReadyForActivation();
			
			UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
				this, OnShootEventTag, nullptr, false, true);

			Task->EventReceived.AddDynamic(this, &ThisClass::OnAttackGameplayEventReceivedByRange);
			Task->ReadyForActivation();
		}
		break;
	}

	UAbilityTask_WaitGameplayEvent* ResetTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, TwoMinGameplayTag::Shared_Event_ResetAttackCount, nullptr, false,
	true);

	ResetTask->EventReceived.AddDynamic(this, &ThisClass::OnResetAttackCountGameplayEffectReceive);
	ResetTask->ReadyForActivation();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinEGA_AttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{

	
	if(ATwoMinEnemyDummy* Dummy = Cast<ATwoMinEnemyDummy>(GetAvatarActorFromActorInfo()))
	{
		if (Dummy->GetResetAttackLocation())
		{
			Dummy->SetActorLocation(StartLocation);	
		}
	}

	if (ATwoMinEnemyCharacter* Enemy = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo()))
	{
		Enemy->GetCombatComponent()->ClearOverlappingActors();
	}
	
	ResetComboCount();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinEGA_AttackBase::OnResetAttackCountGameplayEffectReceive(FGameplayEventData Payload)
{
	ResetComboCount();
}

TSubclassOf<UGameplayEffect> UTwoMinEGA_AttackBase::GetAttackGameplayEffectClass() const
{
	return AttackGameplayEffectClass;
}
