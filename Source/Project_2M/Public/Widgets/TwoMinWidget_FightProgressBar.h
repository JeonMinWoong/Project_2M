// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_FightProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_FightProgressBar : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void Init();
	void SetPercent(float InPercent);
	void SetColor(const bool bIsFull, bool bIsOnAngerMode);
	
private:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* FightProgressBar;
};
