// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compnents/BaseComponent.h"
#include "BaseUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangedDelegate, float, NewPercent);

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBaseUIComponent : public UBaseComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FOnPercentChangedDelegate OnCurrentHealthChanged;
};
