// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinEGA_DetectBase.h"

#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/EnemyCombatComponent.h"

class UEnemyCombatComponent;

void UTwoMinEGA_DetectBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	if (!DetectAnimMontage)
	{
		CustomCancelAbility();
		return;
	}
	
	PlayToAnimMontage(DetectAnimMontage);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinEGA_DetectBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(ActorInfo->OwnerActor);
	UEnemyCombatComponent* EnemyCombatComponent = Cast<UEnemyCombatComponent>(EnemyCharacter->GetCombatComponent());
	if (EnemyCombatComponent)
	{
		EnemyCombatComponent->SetIsBattlePossible(true);	
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
