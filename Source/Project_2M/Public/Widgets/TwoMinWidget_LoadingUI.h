// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_LoadingUI.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_LoadingUI : public UTwoMinWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
private:
	UFUNCTION()
	void UpdateLoadingText();
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadingText;
	
	UPROPERTY()
	int32 LoadingCount = 0;
	
	UPROPERTY()
	float UpdateLoadingTime = 0.15f;
	
	FTimerHandle LoadingTimerHandle;

};
