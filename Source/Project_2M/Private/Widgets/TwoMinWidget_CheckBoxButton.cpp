// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_CheckBoxButton.h"

#include "Components/Image.h"

void UTwoMinWidget_CheckBoxButton::SetLocked(bool Locked)
{
	if (Locked)
	{
		LockImage->SetVisibility(ESlateVisibility::Visible);
		FocusImage->SetVisibility(ESlateVisibility::Hidden);
		bIsLocked = true;
	}
	else
	{
		LockImage->SetVisibility(ESlateVisibility::Hidden);
		bIsLocked = false;
	}
}

void UTwoMinWidget_CheckBoxButton::SetCheckBox(bool bOn)
{
	if (bOn)
	{
		CheckImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CheckImage->SetVisibility(ESlateVisibility::Hidden);
	}
	
	bIsCheckBox = bOn;
}

void UTwoMinWidget_CheckBoxButton::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	HighlightFocusSlot(true);
}

void UTwoMinWidget_CheckBoxButton::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		SetFocus();
		return;
	}
	HighlightFocusSlot(false);
}

void UTwoMinWidget_CheckBoxButton::HighlightFocusSlot(bool bOn)
{
	FocusImage->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
