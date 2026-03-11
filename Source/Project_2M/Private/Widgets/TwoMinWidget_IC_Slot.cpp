// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_IC_Slot.h"

#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UTwoMinWidget_IC_Slot::SetFocusSlot(bool bOn)
{
	if (bOn)
	{
		SetFocus();
		FocusBox->SetIsEnabled(true);
		TextBox->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
	else
	{
		FocusBox->SetIsEnabled(false);
		TextBox->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
	}
}

void UTwoMinWidget_IC_Slot::SetLocked(bool bOn)
{
	if (bOn)
	{
		bLocked = true;
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		bLocked = false;
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UTwoMinWidget_IC_Slot::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}

void UTwoMinWidget_IC_Slot::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		SetFocus();
		return;
	}
}
