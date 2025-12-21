// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_FightBar.generated.h"

class UTwoMinWidget_FightProgressBar;
class UProgressBar;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_FightBar : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetPercent(float NewPercent);
	void SetOnAngerMode(bool NewOnAngerMode);
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* GridPanel;
	
	UPROPERTY()
	TArray<UTwoMinWidget_FightProgressBar*> ProgressBars;
	
	UPROPERTY()
	int32 MaxFightBarCount;
	
	UPROPERTY()
	bool bIsOnAngerMode;
};
