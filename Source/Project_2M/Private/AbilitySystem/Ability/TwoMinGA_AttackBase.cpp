// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/AutoTargetingComponent.h"
#include "Compnents/Combat/BaseCombatComponent.h"

UTwoMinGA_AttackBase::UTwoMinGA_AttackBase()
{
	bRetriggerInstancedAbility = UTwoMinGA_AttackBase::bIsReTriggerSameAbility();
	AbilityInputType = ETwoAbilityInputType::ReTriggerable;
}


void UTwoMinGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	if (AttackMontages.IsEmpty())
	{
		CustomCancelAbility();
		return;
	}
	
	if (CurComboCount > MaxComboCount)
	{
		ResetComboCount();
	}

	if (!AttackMontages.Contains(CurComboCount))
	{
		CustomCancelAbility();
		return;
	}
	
	if (UAnimMontage* MontageToPlay = AttackMontages[CurComboCount])
	{
		ATwoMinPlayerCharacter* Player = Cast<ATwoMinPlayerCharacter>(ActorInfo->OwnerActor);
		if (Player)
		{
			Player->CancelInputToggle();
			if (UAutoTargetingComponent* AutoTargeting = Player->GetCombatComponent()->GetAutoTargetingComponent())
			{
				AutoTargeting->StartAutoTargeting();
			}
		}
		
		//bIsReTriggerAble = false;
		PlayToAnimMontage(MontageToPlay);

		UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, OnHitEventTag, nullptr, false, true);

		Task->EventReceived.AddDynamic(this, &ThisClass::OnAttackGameplayEventReceivedByMelee);
		Task->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* ResetTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, TwoMinGameplayTag::Shared_Event_ResetAttackCount, nullptr, false,
	true);

		ResetTask->EventReceived.AddDynamic(this, &ThisClass::OnResetAttackCountGameplayEffectReceive);
		ResetTask->ReadyForActivation();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_AttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateEndAbility, bool bWasCancelled)
{
	ATwoMinPlayerCharacter* Player = Cast<ATwoMinPlayerCharacter>(ActorInfo->OwnerActor);
	if (Player)
	{
		if (UAutoTargetingComponent* AutoTargeting = Player->GetCombatComponent()->GetAutoTargetingComponent())
		{
			AutoTargeting->EndAutoTargeting();
		}

		Player->GetCombatComponent()->ClearOverlappingActors();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UTwoMinGA_AttackBase::bIsReTriggerSameAbility() const
{
	return MaxComboCount > 1;
}

void UTwoMinGA_AttackBase::CustomCancelAbility()
{
	Super::CustomCancelAbility();
	ResetComboCount();
}

void UTwoMinGA_AttackBase::OnResetAttackCountGameplayEffectReceive(FGameplayEventData Payload)
{
	ResetComboCount();
}

TSubclassOf<UGameplayEffect> UTwoMinGA_AttackBase::GetAttackGameplayEffectClass() const
{
	return AttackGameplayEffectClass;
}

void UTwoMinGA_AttackBase::AddComboCount()
{
	CurComboCount++;
	bIsReTriggerAble = true;
}

void UTwoMinGA_AttackBase::ResetComboCount()
{
	CurComboCount = 1;
	bIsReTriggerAble = false;
}

void UTwoMinGA_AttackBase::ConversionComboCount(const int32 InComboCount)
{
	CurComboCount = InComboCount;
}
