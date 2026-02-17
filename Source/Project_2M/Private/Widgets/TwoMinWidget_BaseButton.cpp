// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_BaseButton.h"

#include "Components/Image.h"

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