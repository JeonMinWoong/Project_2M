// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_PossibleExecution.h"

#include "TwoMinFunctionLibrary.h"
#include "Components/CanvasPanel.h"

void UTwoMinWidget_PossibleExecution::ShowPossibleExecution()
{
	const bool bIsUsingGamePad = UTwoMinFunctionLibrary::IsUsingGamePad(GetWorld(), GetOwningPlayer()->GetPlatformUserId());
	if (bIsUseGamepad == bIsUsingGamePad) return;
	
	bIsUseGamepad = bIsUsingGamePad;
	
	if (bIsUseGamepad)
	{
		KeyBoardCanvas->SetVisibility(ESlateVisibility::Hidden);
		GamepadCanvas->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		KeyBoardCanvas->SetVisibility(ESlateVisibility::Visible);
		GamepadCanvas->SetVisibility(ESlateVisibility::Hidden);
	}
}
