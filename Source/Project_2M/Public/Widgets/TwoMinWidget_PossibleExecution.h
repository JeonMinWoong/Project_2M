// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_PossibleExecution.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_PossibleExecution : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void ShowPossibleExecution();

protected:
	virtual void OnInputDeviceChanged(bool bIsGamePad) override;

private:
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* KeyBoardCanvas;
	
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* GamepadCanvas;
	
	UPROPERTY()
	bool bIsUseGamepad = false;
};
