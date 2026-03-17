// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_StoreSelect.h"

#include "MathUtil.h"
#include "Components/VerticalBox.h"
#include "Widgets/TwoMinWidget_InventorySelect.h"
#include "Widgets/TwoMinWidget_SelectSlot.h"

void UTwoMinWidget_StoreSelect::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	InitStoreSelectSlots(InventoryToStore, InventoryToStoreSlots);
	InitStoreSelectSlots(StoreToInventory, StoreToInventorySlots);
	InitStoreSelectSlots(NoDeal, NoDealSlots);
}

void UTwoMinWidget_StoreSelect::InitStoreSelectSlots(const UVerticalBox* VerticalBox, 
	TArray<TObjectPtr<UTwoMinWidget_SelectSlot>>& SlotGroup)
{
	if (!VerticalBox) return;
	
	TArray<UWidget*> IInventoryToConsumeSlotGroup = VerticalBox->GetAllChildren();
	if (IInventoryToConsumeSlotGroup.IsEmpty()) return;

	for (UWidget* InventorySlot : IInventoryToConsumeSlotGroup)
	{
		if (UTwoMinWidget_SelectSlot* CastSlot = Cast<UTwoMinWidget_SelectSlot>(InventorySlot))
		{
			CastSlot->InitSlot();
			SlotGroup.Add(CastSlot);
		}
	}
}

int32 UTwoMinWidget_StoreSelect::GetMaxSelectSlotIndex() const
{
	switch (StoreSelectType) {
	case EStoreSelectType::InventoryToStore:
		return InventoryToStoreSlots.Num() - 1;
	case EStoreSelectType::StoreToInventory:
		return StoreToInventorySlots.Num() - 1;
	case EStoreSelectType::NoDeal:
		return NoDealSlots.Num() - 1;
	}
	
	return 0;
}

void UTwoMinWidget_StoreSelect::SetStoreSelectType(EStoreSelectType NewStoreSelectType)
{
	bIsOpen = true;
	StoreSelectType = NewStoreSelectType;
	
	ESlateVisibility InventoryVisibility = ESlateVisibility::Hidden;
	ESlateVisibility StoreVisibility = ESlateVisibility::Hidden;
	ESlateVisibility NoDealVisibility = ESlateVisibility::Hidden;

	switch (StoreSelectType) {
	case EStoreSelectType::InventoryToStore:
		InventoryVisibility = ESlateVisibility::Visible;
		break;
	case EStoreSelectType::StoreToInventory:
		StoreVisibility = ESlateVisibility::Visible;
		break;
	case EStoreSelectType::NoDeal:
		NoDealVisibility = ESlateVisibility::Visible;
		break;
	}
	
	InventoryToStore->SetVisibility(InventoryVisibility);
	StoreToInventory->SetVisibility(StoreVisibility);
	NoDeal->SetVisibility(NoDealVisibility);
	
	OnFocusFirstSlot();
}

void UTwoMinWidget_StoreSelect::SetStoreSelectIndex(const int32 NextIndex)
{
	CurSelectSlotIndex = FMathf::Clamp(CurSelectSlotIndex + NextIndex, 0, GetMaxSelectSlotIndex());
}

int32 UTwoMinWidget_StoreSelect::GetStoreSelectIndex() const
{
	return FMathf::Clamp(CurSelectSlotIndex, 0, GetMaxSelectSlotIndex());
}

void UTwoMinWidget_StoreSelect::OnFocusStoreSelectSlot(const int32 NextIndex)
{
	if (StoreSelectType == EStoreSelectType::InventoryToStore)
	{
		InventoryToStoreSlots[NextIndex]->SetFocus();
	}
	else if (StoreSelectType == EStoreSelectType::StoreToInventory)
	{
		StoreToInventorySlots[NextIndex]->SetFocus();
	}
	else if (StoreSelectType == EStoreSelectType::NoDeal)
	{
		NoDealSlots[NextIndex]->SetFocus();
	}
}

void UTwoMinWidget_StoreSelect::QuitInventorySelect()
{
	SetVisibility(ESlateVisibility::Hidden);
	
	bIsOpen = false;
	CurSelectSlotIndex = 0;
	
	ResetAllSelectSlot();
}

ESelectEventType UTwoMinWidget_StoreSelect::OnTriggerEvent(int32 CurIndex)
{
	if (StoreSelectType == EStoreSelectType::InventoryToStore)
	{
		if (CurIndex == 0)
		{
			InventoryToStoreSlots[CurIndex]->SelectSlot(false);
			return ESelectEventType::InventoryToStore_Sell;
		}
		else if (CurIndex == 1)
		{
			InventoryToStoreSlots[CurIndex]->SelectSlot(true);
			return ESelectEventType::OpenItemPopup;
		}
		
		InventoryToStoreSlots[CurIndex]->SelectSlot(false);
		return ESelectEventType::Cancel;
	}
	else if (StoreSelectType == EStoreSelectType::StoreToInventory)
	{
		if (CurIndex == 0)
		{
			StoreToInventorySlots[CurIndex]->SelectSlot(false);
			return ESelectEventType::StoreToInventory_Buy;
		}
		else if (CurIndex == 1)
		{
			StoreToInventorySlots[CurIndex]->SelectSlot(true);
			return ESelectEventType::OpenItemPopup;
		}
		
		StoreToInventorySlots[CurIndex]->SelectSlot(false);
		return ESelectEventType::Cancel;
	}
	else if (StoreSelectType == EStoreSelectType::NoDeal)
	{
		if (CurIndex == 0)
		{
			NoDealSlots[CurIndex]->SelectSlot(true);
			return ESelectEventType::OpenItemPopup;
		}
		
		NoDealSlots[CurIndex]->SelectSlot(false);
		return ESelectEventType::Cancel;
	}
	
	return ESelectEventType::Cancel;
}

void UTwoMinWidget_StoreSelect::ResetAllSelectSlot()
{
	TArray<UTwoMinWidget_SelectSlot*> SlotGroup;
	if (StoreSelectType == EStoreSelectType::InventoryToStore)
	{
		SlotGroup = InventoryToStoreSlots;
	}
	else if (StoreSelectType == EStoreSelectType::StoreToInventory)
	{
		SlotGroup = StoreToInventorySlots;
	}
	else if (StoreSelectType == EStoreSelectType::NoDeal)
	{
		SlotGroup = NoDealSlots;
	}
	
	for (auto SelectSlot : SlotGroup)
	{
		SelectSlot->SelectSlot(false);
	}
}

void UTwoMinWidget_StoreSelect::OnFocusFirstSlot()
{
	if (StoreSelectType == EStoreSelectType::InventoryToStore)
	{
		InventoryToStoreSlots[0]->SetFocus();
	}
	else if (StoreSelectType == EStoreSelectType::StoreToInventory)
	{
		StoreToInventorySlots[0]->SetFocus();
	}
	else if (StoreSelectType == EStoreSelectType::NoDeal)
	{
		NoDealSlots[0]->SetFocus();
	}
}
