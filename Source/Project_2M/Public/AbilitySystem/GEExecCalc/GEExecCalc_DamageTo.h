// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExecCalc_DamageTo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UGEExecCalc_DamageTo : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGEExecCalc_DamageTo();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	bool HasStateTag(const UAbilitySystemComponent* TargetASC, const FGameplayTag GameplayTag) const;

	UPROPERTY()
	FGameplayTag SourceUnBreakAttackTag;
	
	UPROPERTY()
	FGameplayTag TargetGuardStateTag;

	UPROPERTY()
	FGameplayTag TargetInvincibleStateTag;
};
