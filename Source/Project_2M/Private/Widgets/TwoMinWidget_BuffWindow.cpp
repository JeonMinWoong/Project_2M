// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_BuffWindow.h"

#include "Components/GridPanel.h"
#include "Widgets/TwoMinWidget_BuffSlot.h"

void UTwoMinWidget_BuffWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!BuffGrid) return;
	
	TArray<UWidget*> BuffSlotGroup = BuffGrid->GetAllChildren();
	if (BuffSlotGroup.IsEmpty()) return;

	int32 SlotIndex = 0;
	for (UWidget* BuffSlot : BuffSlotGroup)
	{
		if (UTwoMinWidget_BuffSlot* CastSlot = Cast<UTwoMinWidget_BuffSlot>(BuffSlot))
		{
			CastSlot->Init(this, SlotIndex);
			BuffSlots.Add(CastSlot);
		}
		
		SlotIndex++;
	}
	
	MaxSlotIndex = BuffSlots.Num() - 1;
}

void UTwoMinWidget_BuffWindow::SetBuffSlot(const int32 ItemID)
{
	// 이미 있는 버프면 초기화.
	for (int Index = 0; Index < MaxSlotIndex; ++Index)
	{
		if (BuffSlots[Index]->GetBuffItemID() == ItemID)
		{
			BuffSlots[Index]->OffBuff();
			break;
		}
	}
	
	// 빈 슬롯에 추가.
	for (int Index = 0; Index < MaxSlotIndex; ++Index)
	{
		if (BuffSlots[Index]->GetBuffItemID() == 0)
		{
			BuffSlots[Index]->OnBuff(ItemID);
			break;
		}
	}
}

void UTwoMinWidget_BuffWindow::SortBuffSlots(int32 OffSlotIndex)
{
	int32 StartSlotIndex = OffSlotIndex;
	int32 EndSlotIndex = 0;

	for (int Index = StartSlotIndex + 1; Index < MaxSlotIndex; ++Index)
	{
		if (BuffSlots[Index]->GetBuffItemID() == 0)
		{
			EndSlotIndex = Index;
			break;
		}
	}
	
	if (EndSlotIndex == 0) return;
	
	for (int Index = StartSlotIndex; Index < EndSlotIndex; ++Index)
	{
		int32 NextIndex = Index + 1;
		UTwoMinWidget_BuffSlot* NextSlot = BuffSlots[NextIndex];
		if (NextSlot->GetBuffItemID() == 0) break;
		
		BuffSlots[Index]->OnCopyBuff(NextSlot->GetBuffItemID(), NextSlot->GetRunningTime());
		NextSlot->OffBuff();
	}
}
