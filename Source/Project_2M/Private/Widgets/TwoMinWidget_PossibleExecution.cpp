// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_PossibleExecution.h"

#include "Components/CanvasPanel.h"

void UTwoMinWidget_PossibleExecution::ShowPossibleExecution()
{
	OnInputDeviceChanged(IsUsingGamePad());
}

void UTwoMinWidget_PossibleExecution::OnInputDeviceChanged(bool bIsGamePad)
{
	if (bIsUseGamepad == bIsGamePad) return;

	bIsUseGamepad = bIsGamePad;

	KeyBoardCanvas->SetVisibility(bIsGamePad ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	GamepadCanvas->SetVisibility(bIsGamePad ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
