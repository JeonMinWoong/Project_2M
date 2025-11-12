// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinAbilityTask.h"
#include "TwoMinAT_ExecutionAutoPosition.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExecutionAutoPositionDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinAT_ExecutionAutoPosition : public UTwoMinAbilityTask
{
	GENERATED_BODY()

public:
	static UTwoMinAT_ExecutionAutoPosition* CreateTickTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FExecutionAutoPositionDelegate OnExecutionAutoPositionTick;

	UPROPERTY()
	float MaxDuration;

	UPROPERTY()
	float CurrentDuration = 0;
};
