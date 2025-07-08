// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_Roll_Player.h"

#include "TwoMinDebugHelper.h"
#include "Character/TwoMinPlayerCharacter.h"

void UTwoMinGA_Roll_Player::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	DebugTwoMin::Print(TEXT("Roll Ability Activated"), FColor::Green, 1);
	
	StartRoll(ActorInfo);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_Roll_Player::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	DebugTwoMin::Print(TEXT("Roll Ability End"), FColor::Blue, 2);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_Roll_Player::StartRoll(const FGameplayAbilityActorInfo* ActorInfo)
{
	if (!RollMontages)
	{
		return;
	}

	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(ActorInfo->OwnerActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->CancelInputToggle();
	}
	
	PlayToAnimMontage(RollMontages);
}