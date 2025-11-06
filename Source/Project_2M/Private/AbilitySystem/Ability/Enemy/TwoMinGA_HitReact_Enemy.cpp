// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinGA_HitReact_Enemy.h"

#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Character/TwoMinEnemyCharacter.h"

void UTwoMinGA_HitReact_Enemy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	StartDecreaseGroggy();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

int32 UTwoMinGA_HitReact_Enemy::GetPlayHitReactMontageNumber(const FVector OwnerForward, const FVector ToImpact,
                                                             const FAttackInfoData& AttackInfoData) const
{
	const int HitMontageNumber = Super::GetPlayHitReactMontageNumber(OwnerForward, ToImpact, AttackInfoData);
	if (!HitReactMontages.Contains(HitMontageNumber))
	{
		return 5; // Default Hit Montage Number
	}

	return HitMontageNumber;
}

void UTwoMinGA_HitReact_Enemy::StartDecreaseGroggy()
{
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo());
	if (!EnemyCharacter) return;
	
	UTwoMinFunctionLibrary::RemoveGameplayTagToActor(EnemyCharacter, TwoMinGameplayTag::Enemy_State_DecreaseGroggy);
	
	FTimerManager& TimerManager = EnemyCharacter->GetWorldTimerManager();
	if (TimerManager.IsTimerActive(EnemyCharacter->DecreaseGroggyTimerHandle))
	{
		TimerManager.ClearTimer(EnemyCharacter->DecreaseGroggyTimerHandle);
	}
	
	UTwoMinAbilitySystemComponent* EnemyASC = EnemyCharacter->GetAbilitySystemComponent();
	float DecreaseGroggyDelay = EnemyASC->GetNumericAttribute(UTwoMinAttributeSet::GetDecreaseGroggyDelayAttribute());
	TimerManager.SetTimer(EnemyCharacter->DecreaseGroggyTimerHandle,
		[EnemyCharacter]()
		{
			UTwoMinFunctionLibrary::AddGameplayTagToActor(EnemyCharacter, TwoMinGameplayTag::Enemy_State_DecreaseGroggy);
		}, DecreaseGroggyDelay, false);
}
