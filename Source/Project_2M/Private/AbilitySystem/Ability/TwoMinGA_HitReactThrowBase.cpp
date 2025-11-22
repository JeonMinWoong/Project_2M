// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_HitReactThrowBase.h"

#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UTwoMinGA_HitReactThrowBase::PreActivate(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	FGameplayEventData EventData;
	UTwoMinFunctionLibrary::SendToGameplayEffectEvent(GetAvatarActorFromActorInfo(),
		TwoMinGameplayTag::Shared_Event_ResetAttackCount, EventData);
	
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UTwoMinGA_HitReactThrowBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!HitReactThrowMontages)
	{
		CustomCancelAbility();
		return;
	}
	
	LookDirectionHitPos(TriggerEventData);
	UAbilityTask_PlayMontageAndWait* Task = PlayToAnimMontage(HitReactThrowMontages);
	Task->OnBlendOut.AddDynamic(this, &ThisClass::CustomOnBlendOutAbility);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_HitReactThrowBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
