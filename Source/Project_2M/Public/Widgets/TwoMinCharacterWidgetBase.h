// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinCharacterWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinCharacterWidgetBase : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	virtual void SetCurrentHealthPercent(float Percent);
	
};
