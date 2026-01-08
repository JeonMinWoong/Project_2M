// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TwoMinAbilityTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinAbilityTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	virtual void TickTask(float DeltaTime) override;
};
