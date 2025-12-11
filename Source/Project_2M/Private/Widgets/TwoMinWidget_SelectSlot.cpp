// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_SelectSlot.h"

#include "Components/Image.h"

void UTwoMinWidget_SelectSlot::InitSlot()
{
	SelectImage->SetVisibility(ESlateVisibility::Hidden);
}

void UTwoMinWidget_SelectSlot::SelectSlot(const bool bOn)
{
	SelectImage->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UTwoMinWidget_SelectSlot::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	HighlightInventorySlot(true);
}

void UTwoMinWidget_SelectSlot::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		SetFocus();
		return;
	}
	
	HighlightInventorySlot(false);
}

void UTwoMinWidget_SelectSlot::HighlightInventorySlot(const bool bOn)
{
	FocusImage->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
