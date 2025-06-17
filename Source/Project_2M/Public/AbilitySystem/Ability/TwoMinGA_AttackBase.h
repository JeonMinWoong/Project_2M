// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_AttackBase.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced)
class PROJECT_2M_API UTwoMinGA_AttackBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

public:
	UTwoMinGA_AttackBase();

	UFUNCTION()
	void AddComboCount();
	
	UFUNCTION()
	void ResetComboCount();
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Montages")
	TMap<int, UAnimMontage*> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|Combo")
	int32 MaxComboCount = 1;
 
	UPROPERTY()
	int32 CurComboCount = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|Combo")
	float ComboResetTime = 0.3f;
};
