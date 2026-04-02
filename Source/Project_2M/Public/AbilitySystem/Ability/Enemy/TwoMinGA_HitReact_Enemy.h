// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_HitReactBase.h"
#include "TwoMinGA_HitReact_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_HitReact_Enemy : public UTwoMinGA_HitReactBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual int32 GetPlayHitReactMontageNumber(const FVector OwnerForward, const FVector ToImpact,
		const FAttackInfoData& AttackInfoData) const override;

private:
	void StartDecreaseGroggy();
	void ImmediatelyBattleStart(const FGameplayEventData* TriggerEventData);
};
