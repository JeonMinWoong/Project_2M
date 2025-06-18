// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"

#include "TwoMinDebugHelper.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

void UTwoMinGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	if (AttackMontages.IsEmpty()) return;

	if (CurComboCount > MaxComboCount)
	{
		ResetComboCount();
	}

	if (!AttackMontages.Contains(CurComboCount)) return;


	if (UAnimMontage* MontageToPlay = AttackMontages[CurComboCount])
	{
		bIsReTriggerAble = false;
		PlayToAnimMontage(MontageToPlay);
		RotateTowardsCamera();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_AttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UTwoMinGA_AttackBase::bIsReTriggerSameAbility() const
{
	return MaxComboCount > 1;
}

UTwoMinGA_AttackBase::UTwoMinGA_AttackBase()
{
	bRetriggerInstancedAbility = UTwoMinGA_AttackBase::bIsReTriggerSameAbility();
	AbilityInputType = ETwoAbilityInputType::ReTriggerable;
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
