// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_StoreSlot.h"

#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UTwoMinWidget_StoreSlot::ClearSlot()
{
	SetIsFocusable(false);
	HighlightStoreSlot(false);
	ItemID = 0;
	SlotActiveImageBox->SetVisibility(ESlateVisibility::Hidden);
	SlotImageIcon->SetVisibility(ESlateVisibility::Hidden);
	SlotNameTextBlock->SetText(FText::FromString(""));
	SlotPriceTextBlock->SetText(FText::FromString(""));
}

void UTwoMinWidget_StoreSlot::SetActiveSlot(const FItemData& NewItemData)
{
	SetIsFocusable(true);
	ItemID = NewItemData.ItemID;
	SlotActiveImageBox->SetVisibility(ESlateVisibility::Visible);
	SlotImageIcon->SetVisibility(ESlateVisibility::Visible);
	SlotImageIcon->SetBrushFromTexture(NewItemData.ItemTexture);
	SlotNameTextBlock->SetText(FText::FromString(NewItemData.ItemName));
	
	FString Str = FString::Printf(TEXT("<StoreGold>%d</><img id=\"ClearGoldImage\"/>"), NewItemData.BuyPrice);
	SlotPriceTextBlock->SetText(FText::FromString(Str));
}

void UTwoMinWidget_StoreSlot::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	HighlightStoreSlot(true);
}

void UTwoMinWidget_StoreSlot::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
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
	HighlightStoreSlot(false);
}

void UTwoMinWidget_StoreSlot::HighlightStoreSlot(const bool bOn)
{
	SlotFocusImage->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
