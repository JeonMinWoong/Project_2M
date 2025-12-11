// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_InventorySelect.h"

#include "MathUtil.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "Widgets/TwoMinWidget_SelectSlot.h"

void UTwoMinWidget_InventorySelect::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	InitInventorySelectSlots(InventoryToConsume, InventoryToConsumeSlots);
	InitInventorySelectSlots(QuickToConsume, QuickToConsumeSlots);
	InitInventorySelectSlots(InventoryToEquipment, InventoryToEquipmentSlots);
}

void UTwoMinWidget_InventorySelect::InitInventorySelectSlots(const UVerticalBox* VerticalBox, 
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

void UTwoMinWidget_InventorySelect::SetInventorySelectType(const EInventorySelectType NewInventorySelectType)
{
	bIsOpen = true;
	InventorySelectType = NewInventorySelectType;
	if (InventorySelectType == EInventorySelectType::InventoryToConsume)
	{
		InventoryToConsume->SetVisibility(ESlateVisibility::Visible);
		QuickToConsume->SetVisibility(ESlateVisibility::Hidden);
		InventoryToEquipment->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (InventorySelectType == EInventorySelectType::QuickToConsume)
	{
		InventoryToConsume->SetVisibility(ESlateVisibility::Hidden);
		QuickToConsume->SetVisibility(ESlateVisibility::Visible);
		InventoryToEquipment->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (InventorySelectType == EInventorySelectType::InventoryToEquipment)
	{
		InventoryToConsume->SetVisibility(ESlateVisibility::Hidden);
		QuickToConsume->SetVisibility(ESlateVisibility::Hidden);
		InventoryToEquipment->SetVisibility(ESlateVisibility::Visible);
	}
	
	SetFocusSlot();
}

void UTwoMinWidget_InventorySelect::SetInventorySelectIndex(const int32 NextIndex)
{
	if (InventorySelectType == EInventorySelectType::InventoryToConsume)
	{
		InventoryToConsumeSlotIndex = FMathf::Clamp(InventoryToConsumeSlotIndex + NextIndex, 
			0, InventoryToConsumeSlots.Num() - 1);
	}
	else if (InventorySelectType == EInventorySelectType::QuickToConsume)
	{
		QuickToConsumeSlotIndex = FMathf::Clamp(QuickToConsumeSlotIndex + NextIndex, 
			0, QuickToConsumeSlots.Num() - 1);
	}
	else if (InventorySelectType == EInventorySelectType::InventoryToEquipment)
	{
		InventoryToEquipmentSlotIndex = FMathf::Clamp(InventoryToEquipmentSlotIndex + NextIndex, 
			0, InventoryToEquipmentSlots.Num() - 1);
	}
}

int32 UTwoMinWidget_InventorySelect::GetInventorySelectIndex() const
{
	if (InventorySelectType == EInventorySelectType::InventoryToConsume)
	{
		return FMathf::Clamp(InventoryToConsumeSlotIndex, 0, InventoryToConsumeSlots.Num() - 1);
	}
	else if (InventorySelectType == EInventorySelectType::QuickToConsume)
	{
		return FMathf::Clamp(QuickToConsumeSlotIndex, 0, QuickToConsumeSlots.Num() - 1);
	}
	else if (InventorySelectType == EInventorySelectType::InventoryToEquipment)
	{
		return FMathf::Clamp(InventoryToEquipmentSlotIndex, 0, InventoryToEquipmentSlots.Num() - 1);
	}
	
	return 0;
}

void UTwoMinWidget_InventorySelect::MoveToInventorySelectSlot(const int32 NextIndex)
{
	if (InventorySelectType == EInventorySelectType::InventoryToConsume)
	{
		InventoryToConsumeSlots[NextIndex]->SetFocus();
	}
	else if (InventorySelectType == EInventorySelectType::QuickToConsume)
	{
		QuickToConsumeSlots[NextIndex]->SetFocus();
	}
	else if (InventorySelectType == EInventorySelectType::InventoryToEquipment)
	{
		InventoryToEquipmentSlots[NextIndex]->SetFocus();
	}
}

void UTwoMinWidget_InventorySelect::QuitInventorySelect()
{
	bIsOpen = false;
	if (InventorySelectType == EInventorySelectType::InventoryToConsume)
	{
		InventoryToConsumeSlotIndex = 0;
		ResetAllSelectSlot();
	}
	else if (InventorySelectType == EInventorySelectType::QuickToConsume)
	{
		QuickToConsumeSlotIndex = 0;
		ResetAllSelectSlot();
	}
	else if (InventorySelectType == EInventorySelectType::InventoryToEquipment)
	{
		InventoryToEquipmentSlotIndex = 0;
		ResetAllSelectSlot();
	}
}

ESelectEventType UTwoMinWidget_InventorySelect::OnTriggerEvent(int32 CurIndex)
{
	if (InventorySelectType == EInventorySelectType::InventoryToConsume)
	{
		if (CurIndex == 0)
		{
			InventoryToConsumeSlots[CurIndex]->SelectSlot(true);
			return ESelectEventType::InventoryToConsume_Register;
		}
		else if (CurIndex == 1)
		{
			InventoryToConsumeSlots[CurIndex]->SelectSlot(false);
			return ESelectEventType::InventoryToConsume_Use;
		}
		
		InventoryToConsumeSlots[CurIndex]->SelectSlot(false);
		return ESelectEventType::Cancel;
	}
	else if (InventorySelectType == EInventorySelectType::QuickToConsume)
	{
		if (CurIndex == 0)
		{
			QuickToConsumeSlots[CurIndex]->SelectSlot(false);
			return ESelectEventType::QuickToConsume_NonRegister;
		}
		else if (CurIndex == 1)
		{
			QuickToConsumeSlots[CurIndex]->SelectSlot(false);
			return ESelectEventType::QuickToConsume_Use;
		}
		
		QuickToConsumeSlots[CurIndex]->SelectSlot(false);
		return ESelectEventType::Cancel;
	}
	else if (InventorySelectType == EInventorySelectType::InventoryToEquipment)
	{
		if (CurIndex == 0)
		{
			InventoryToEquipmentSlots[CurIndex]->SelectSlot(false);
			return ESelectEventType::InventoryToEquipment_Change;
		}
		
		InventoryToEquipmentSlots[CurIndex]->SelectSlot(false);
		return ESelectEventType::Cancel;
	}
	
	return ESelectEventType::Cancel;
}

void UTwoMinWidget_InventorySelect::ResetAllSelectSlot()
{
	TArray<UTwoMinWidget_SelectSlot*> SlotGroup;
	if (InventorySelectType == EInventorySelectType::InventoryToConsume)
	{
		SlotGroup = InventoryToConsumeSlots;
	}
	else if (InventorySelectType == EInventorySelectType::QuickToConsume)
	{
		SlotGroup = QuickToConsumeSlots;
	}
	else if (InventorySelectType == EInventorySelectType::InventoryToEquipment)
	{
		SlotGroup = InventoryToEquipmentSlots;
	}
	
	for (auto SelectSlot : SlotGroup)
	{
		SelectSlot->SelectSlot(false);
	}
}

void UTwoMinWidget_InventorySelect::SetFocusSlot()
{
	if (InventorySelectType == EInventorySelectType::InventoryToConsume)
	{
		InventoryToConsumeSlots[0]->SetFocus();
	}
	else if (InventorySelectType == EInventorySelectType::QuickToConsume)
	{
		QuickToConsumeSlots[0]->SetFocus();
	}
	else if (InventorySelectType == EInventorySelectType::InventoryToEquipment)
	{
		InventoryToEquipmentSlots[0]->SetFocus();
	}
}