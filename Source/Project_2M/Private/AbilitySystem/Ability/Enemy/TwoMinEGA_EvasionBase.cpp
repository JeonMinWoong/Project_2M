// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinEGA_EvasionBase.h"

void UTwoMinEGA_EvasionBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (EvasionAnimMontage.IsEmpty())
	{
		CustomCancelAbility();
		return;
	}

	EEnemyEvasionType EvasionType = 
		static_cast<EEnemyEvasionType>(static_cast<int32>(TriggerEventData->EventMagnitude));
	
	if (!EvasionAnimMontage.Contains(EvasionType))
	{
		CustomCancelAbility();
		return;
	}
	
	PlayToAnimMontage(EvasionAnimMontage[EvasionType]);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinEGA_EvasionBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
