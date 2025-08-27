// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_HitReactBase.generated.h"

class ATwoMinBaseCharacter;
struct FAttackInfoData;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_HitReactBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	virtual int32 GetPlayHitReactMontageNumber(const FVector OwnerForward, const FVector ToImpact,
		const FAttackInfoData& AttackInfoData) const;

	UPROPERTY(EditDefaultsOnly, Category = "HitReact|Montages")
	TMap<int32, UAnimMontage*> HitReactMontages;
};
