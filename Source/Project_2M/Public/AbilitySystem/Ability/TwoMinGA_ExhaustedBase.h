// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_ExhaustedBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_ExhaustedBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Guard|Montages")
	UAnimMontage* ExhaustedAnimMontage;
};
