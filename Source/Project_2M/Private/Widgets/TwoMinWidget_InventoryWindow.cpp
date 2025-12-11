// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_InventoryWindow.h"

#include "MathUtil.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "Widgets/TwoMinWidget_InventorySlot.h"

UTwoMinWidget_InventoryWindow::UTwoMinWidget_InventoryWindow()
{
	InventoryWindowType = EInventoryWindowType::Inventory;
}

void UTwoMinWidget_InventoryWindow::ClearAllSlots()
{
	for (auto InventorySlot : InventorySlots)
	{
		InventorySlot->ClearSlot();
	}
}

void UTwoMinWidget_InventoryWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!InventoryGrid) return;
	
	TArray<UWidget*> InventorySlotGroup = InventoryGrid->GetAllChildren();
	if (InventorySlotGroup.IsEmpty()) return;

	for (UWidget* InventorySlot : InventorySlotGroup)
	{
		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(InventorySlot->Slot);
		if (!GridSlot) continue;
		
		MaxRowIndex = FMathf::Max(MaxRowIndex, GridSlot->GetRow());
		MaxColumnIndex = FMathf::Max(MaxColumnIndex, GridSlot->GetColumn());
		
		if (UTwoMinWidget_InventorySlot* CastSlot = Cast<UTwoMinWidget_InventorySlot>(InventorySlot))
		{
			CastSlot->InitSlot();
			InventorySlots.Add(CastSlot);
		}
	}
	
	CurInventoryIndex = 0;
	MaxInventoryIndex = InventorySlotGroup.Num() - 1;
	CurInventorySlot = Cast<UTwoMinWidget_InventorySlot>(InventorySlotGroup[CurInventoryIndex]);
}
