// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_ExecutionBase.h"

#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"

void UTwoMinGA_ExecutionBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_ExecutionBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UExecutionComponent* UTwoMinGA_ExecutionBase::GetExecutionComponent()
{
	AActor* MyActor = GetAvatarActorFromActorInfo();
	ATwoMinBaseCharacter* BaseCharacter = Cast<ATwoMinBaseCharacter>(MyActor);
	if (!BaseCharacter)
	{
		CustomCancelAbility();
		return nullptr;
	}
	
	UBaseCombatComponent* CombatComponent = BaseCharacter->GetCombatComponent();
	if (!CombatComponent)
	{
		CustomCancelAbility();
		return nullptr;
	}

	UExecutionComponent* ExecutionComponent = CombatComponent->GetExecutionComponent();
	if (!ExecutionComponent)
	{
		CustomCancelAbility();
		return nullptr;
	}

	return ExecutionComponent;
}
