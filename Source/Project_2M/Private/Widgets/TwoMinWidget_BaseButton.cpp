// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_BaseButton.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTwoMinWidget_BaseButton::SetLocked(bool Locked)
{
	if (Locked)
	{
		LockImage->SetVisibility(ESlateVisibility::Visible);
		FLinearColor tColor = FLinearColor(0.072f, 0.072f, 0.072f, 1.f);
		TextBox->SetColorAndOpacity(FSlateColor(tColor));
		bIsLocked = true;
		SetKeepSelect(false);
	}
	else
	{
		LockImage->SetVisibility(ESlateVisibility::Hidden);
		TextBox->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		bIsLocked = false;
		SetKeepSelect(false);
	}
}

void UTwoMinWidget_BaseButton::SetKeepSelect(bool bOn)
{
	if (bIsKeepSelect == false) return; 
	
	if (bOn)
	{
		SelectImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SelectImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTwoMinWidget_BaseButton::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	HighlightFocusSlot(true);
}

void UTwoMinWidget_BaseButton::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		SetFocus();
		return;
	}
	HighlightFocusSlot(false);
}

void UTwoMinWidget_BaseButton::HighlightFocusSlot(const bool bOn)
{
	FocusImage->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}