// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "PlayerUIComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UPlayerUIComponent : public UBaseUIComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FOnPercentChangedDelegate OnCurrentStaminaChanged;

	UPROPERTY()
	FOnPercentChangedDelegate OnCurrentExperienceChanged;
};
