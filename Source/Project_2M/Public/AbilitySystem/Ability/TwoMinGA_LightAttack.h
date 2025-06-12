// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_LightAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_LightAttack : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.
	
	void StartComboTimer();

	UFUNCTION()
	void CancelComboTimer();
	
	void ResetComboCount();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Montages")
	TMap<int, UAnimMontage*> LightAttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|Combo")
	int32 MaxComboCount = 1;
 
	UPROPERTY()
	int32 CurComboCount = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|Combo")
	float ComboResetTime = 0.3f;

	UPROPERTY()
	FTimerHandle StartComboTimerHandle;
};
