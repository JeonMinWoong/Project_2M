// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_IC_Slot.h"

#include "TwoMinFunctionLibrary.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Widgets/TwoMinWidget_KeyType.h"

void UTwoMinWidget_IC_Slot::SetFocusSlot(bool bOn)
{
	if (bOn)
	{
		SetFocus();
		
		OnEnable();
		FocusKey->SetVisibility(ESlateVisibility::Visible);
		FocusKey->SetIsEnabled(true);
		TextBox->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
	else
	{
		FocusKey->SetVisibility(ESlateVisibility::Hidden);
		FocusKey->SetIsEnabled(false);
		TextBox->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
	}
}

void UTwoMinWidget_IC_Slot::SetLocked(bool bOn)
{
	if (bOn)
	{
		bLocked = true;
		FocusKey->SetVisibility(ESlateVisibility::Collapsed);
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
	if (InFocusEvent.GetCause() == EFocusCause::Navigation)
	{
		SetFocus();
		return;
	}
	
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		SetFocus();
		return;
	}
}

void UTwoMinWidget_IC_Slot::OnEnable()
{
	const bool bIsUsingGamePad = UTwoMinFunctionLibrary::IsUsingGamePad(GetWorld(), GetOwningPlayer()->GetPlatformUserId());
	const FString KeyText = bIsUsingGamePad ? TEXT("A") : TEXT("F");
	FocusKey->SetKeyTextValue(KeyText);
}
