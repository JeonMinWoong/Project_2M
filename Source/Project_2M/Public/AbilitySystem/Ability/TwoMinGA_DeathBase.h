// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinGA_DeathBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_DeathBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~ End UGameplayAbility Interface

private:
	UFUNCTION()
	int32 GetDeathMontageNumber() const;

	UFUNCTION()
	void CustomBlendOutAbility();
	
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TMap<int, UAnimMontage*> DeathAnimMontageMap;

	UPROPERTY(EditDefaultsOnly, Category = "RemoveDelay")
	float RemoveDelay = -1.f;
};
