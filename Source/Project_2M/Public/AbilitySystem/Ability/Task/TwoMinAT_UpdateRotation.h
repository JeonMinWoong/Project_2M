// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Task/TwoMinAbilityTask.h"
#include "TwoMinAT_UpdateRotation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateRotationTickDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinAT_UpdateRotation : public UTwoMinAbilityTask
{
	GENERATED_BODY()
	
public:
	static UTwoMinAT_UpdateRotation* CreateTickTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateRotationTickDelegate OnUpdateRotationTick;

	UPROPERTY()
	float MaxDuration;

private:
	UPROPERTY()
	float CurrentDuration = 0;
};
