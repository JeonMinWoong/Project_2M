// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinGA_Death_Enemy.h"

#include "Character/TwoMinEnemyCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"

void UTwoMinGA_Death_Enemy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	AActor* InstigatorActor = const_cast<AActor*>(TriggerEventData->Instigator.Get());
	if (!InstigatorActor) return;
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(InstigatorActor);
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo());
	if (PlayerCharacter && EnemyCharacter)
	{
		PlayerCharacter->GetAbilitySystemComponent()->GiveExperience(EnemyCharacter);
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_Death_Enemy::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
