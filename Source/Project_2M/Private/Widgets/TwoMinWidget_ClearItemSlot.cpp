// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_ClearItemSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinWidget_ClearItemSlot::InitSlot()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UTwoMinWidget_ClearItemSlot::SetClearItemSlotInfo(FItemData ItemData)
{
	SetVisibility(ESlateVisibility::Visible);
	ItemIconImage->SetBrushFromTexture(ItemData.ItemTexture);
	
	const FText ItemCount = FText::FromString(FString::FromInt(ItemData.CurrentCount));
	ItemCountTextBlock->SetText(ItemCount);
	
	const FText ItemName = GetTruncateItemName(ItemData.ItemName);
	ItemNameTextBlock->SetText(ItemName);
}

FText UTwoMinWidget_ClearItemSlot::GetTruncateItemName(const FString& ItemName) const
{
	if (ItemName.Len() > MaxItemNameLength)
	{
		const FString NewStr = ItemName.Left(MaxItemNameLength) + TEXT("...");
		return FText::FromString(NewStr);
	}
	
	return FText::FromString(ItemName);
}
