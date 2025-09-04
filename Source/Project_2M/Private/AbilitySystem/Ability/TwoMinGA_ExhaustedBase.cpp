// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_ExhaustedBase.h"

#include "TwoMinDebugHelper.h"

void UTwoMinGA_ExhaustedBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	TwoMinDebugHelper::Print(TEXT("Exhausted Ability Activated"), FColor::Red);
	
	if (!ExhaustedAnimMontage)
	{
		CustomCancelAbility();
		return;
	}
	
	PlayToAnimMontage(ExhaustedAnimMontage);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
