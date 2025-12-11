// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_InventorySlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ToMinTypes/TwoMinEnumTypes.h"

UTwoMinWidget_InventorySlot::UTwoMinWidget_InventorySlot()
{
	InventorySlotType = EInventorySlotType::Inventory;
}

void UTwoMinWidget_InventorySlot::InitSlot()
{
	ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
	ItemCountText->SetVisibility(ESlateVisibility::Hidden);
	RegisterText->SetVisibility(ESlateVisibility::Hidden);
}

void UTwoMinWidget_InventorySlot::ClearSlot()
{
	ItemInstance = FItemInstance();
	ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
	ItemCountText->SetVisibility(ESlateVisibility::Hidden);
	RegisterText->SetVisibility(ESlateVisibility::Hidden);
}

void UTwoMinWidget_InventorySlot::SetInventorySlot(FItemInstance NewItemInstance)
{
	ItemInstance = NewItemInstance;
	
	ItemIconImage->SetVisibility(ESlateVisibility::Visible);
	if (InventorySlotType != EInventorySlotType::Equipment)
	{
		ItemCountText->SetVisibility(ESlateVisibility::Visible);
	}
	
	ItemIconImage->SetBrushFromTexture(ItemInstance.ItemTexture, false);
	
	FText Text = FText::FromString(FString::FromInt(ItemInstance.HoldCount));
	ItemCountText->SetText(Text);
}

bool UTwoMinWidget_InventorySlot::IsRegister()
{
	return ItemInstance.bIsRegister;
}

void UTwoMinWidget_InventorySlot::OnRegister(bool bIsRegister, EInventorySlotType SlotType, int32 RegisterCount)
{
	ItemInstance.bIsRegister = bIsRegister;
	ItemInstance.SlotType = SlotType;
	ItemInstance.RegisterCount = RegisterCount;
	if (SlotType == EInventorySlotType::Quick)
	{
		InventoryRegister(bIsRegister, EItemType::Consume, RegisterCount + 1);	
	}
	else if (SlotType == EInventorySlotType::Equipment)
	{
		InventoryRegister(bIsRegister, EItemType::Equipment, RegisterCount);
	}
}

int32 UTwoMinWidget_InventorySlot::UnRegister()
{
	ItemInstance.bIsRegister = false;
	ItemInstance.SlotType = EInventorySlotType::Inventory;
	ItemInstance.RegisterCount = -1;
	
	InventoryRegister(false, EItemType::None, -1);
	return 0;
}

void UTwoMinWidget_InventorySlot::SetRinkItemID(int32 InventorySlotIndex)
{
	
}

void UTwoMinWidget_InventorySlot::InventoryRegister(bool bOn, EItemType ItemType, int32 InventorySlotIndex)
{
	RegisterText->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
	if (InventorySlotIndex < 0) return;
	
	FString Str = "";

	if (ItemType == EItemType::Equipment)
	{
		Str = FString::Printf(TEXT("E"));
	}
	else if (ItemType == EItemType::Consume)
	{
		Str = FString::Printf(TEXT("Q%d"), InventorySlotIndex);
	}

	FText Text = FText::FromString(Str);
	RegisterText->SetText(Text);
}

void UTwoMinWidget_InventorySlot::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	HighlightInventorySlot(true);
}

void UTwoMinWidget_InventorySlot::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		SetFocus();
		return;
	}
	HighlightInventorySlot(false);
}

void UTwoMinWidget_InventorySlot::HighlightInventorySlot(const bool bOn)
{
	FocusImage->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
