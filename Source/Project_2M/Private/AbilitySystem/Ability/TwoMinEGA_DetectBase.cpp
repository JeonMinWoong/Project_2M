// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinEGA_DetectBase.h"

#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "Compnents/UI/EnemyUIComponent.h"
#include "GameInstance/TwoMinGameInstance.h"

class UEnemyCombatComponent;

void UTwoMinEGA_DetectBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(ActorInfo->OwnerActor);
	if (UEnemyCombatComponent* EnemyCombatComponent = Cast<UEnemyCombatComponent>(EnemyCharacter->GetCombatComponent()))
	{
		EnemyCombatComponent->SetIsBattlePossible(true);
		EnemyCombatComponent->SetIsEquip(true);
	}

	if (EnemyCharacter->IsUseBossHealthBar())
	{
		EnemyCharacter->BossDetectProcess();
	}
	
	if (DetectAnimMontage)
	{
		PlayToAnimMontage(DetectAnimMontage);
	}
	else
	{
		// 애니메이션이 없을 경우도 있다. ex) 궁수 : 즉시 종료.
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinEGA_DetectBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
