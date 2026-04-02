// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinGA_HitReact_Enemy.h"

#include "AIController.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "Controller/TwoMinEnemyAIController.h"

void UTwoMinGA_HitReact_Enemy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	StartDecreaseGroggy();
	ImmediatelyBattleStart(TriggerEventData);
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

void UTwoMinGA_HitReact_Enemy::ImmediatelyBattleStart(const FGameplayEventData* TriggerEventData)
{
	const AActor* Attacker = TriggerEventData->Instigator.Get();
	ATwoMinEnemyCharacter* VictimEnemyCharacter = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo());
	if (!Attacker || !VictimEnemyCharacter) return;
	
	UEnemyCombatComponent* CombatComponent = Cast<UEnemyCombatComponent>(VictimEnemyCharacter->GetCombatComponent());
	if (!CombatComponent) return;
	
	const float BattleRange = CombatComponent->GetBaseBattleRange();
	const float CurDist = VictimEnemyCharacter->GetDistanceTo(Attacker);
	if (CurDist > BattleRange) return;
	
	CombatComponent->SetIsBattlePossible(true);
}
