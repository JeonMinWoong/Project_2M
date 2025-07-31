// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_HeavyAttack_Player.h"

#include "TwoMinDebugHelper.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Character/TwoMinPlayerCharacter.h"

void UTwoMinGA_HeavyAttack_Player::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (HeavyAttackComboType == EHeavyAttackComboType::MustBeLinkedToLightAttack)
	{
		// Todo: 강 공격 단일로 안되게 하기.
		//
		// ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(ActorInfo->OwnerActor);
		// if (!PlayerCharacter)
		// {
		// 	CustomCancelAbility();
		// 	return;
		// }
		//
		// if (PlayerCharacter->GetAbilitySystemComponent()->IsPlayingAbility(MustBeLinkedTag) == false)
		// {
		// 	CustomCancelAbility();
		// 	return;
		// }
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_HeavyAttack_Player::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
