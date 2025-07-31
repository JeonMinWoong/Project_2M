// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "TwoMinGA_HeavyAttack_Player.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_HeavyAttack_Player : public UTwoMinGA_AttackBase
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

private:
	UPROPERTY(EditDefaultsOnly, Category = "Attack|ComboType")
	EHeavyAttackComboType HeavyAttackComboType = EHeavyAttackComboType::MustBeLinkedToLightAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|MustBeLinkedTag")
	FGameplayTag MustBeLinkedTag;
};
