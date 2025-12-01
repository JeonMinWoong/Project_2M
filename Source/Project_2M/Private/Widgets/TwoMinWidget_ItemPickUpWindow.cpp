// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_ItemPickUpWindow.h"

#include "Components/VerticalBox.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidget_ItemPickUpSlot.h"


void UTwoMinWidget_ItemPickUpWindow::OnItemPickUpSlot(int32 ShowAllItemCount, TArray<FItemPickUpEntry>& ItemList)
{
	if (ItemPickUpSlotGroup.IsEmpty()) return;
	
	// todo:  최대치 제한.
	for (int Index = 0; Index < ShowAllItemCount; ++Index)
	{
		float Delay = FMath::Max(0.01f, SlotTerm * Index);
		
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, 
			[this, ItemList, Index]()
		{
			ShowItemPickUpSlot(ItemList[Index].ItemName, ItemList[Index].ItemCount, ItemList[Index].ItemTexture);	
		}, Delay, false);
	}
}

void UTwoMinWidget_ItemPickUpWindow::ShowItemPickUpSlot(const FString& ItemName, int32 ItemCount, UTexture2D* ItemTexture)
{
	int32 CopySlotCount = -1;
	for (int Index = 0; Index <= MaxItemPickUpSlotCount; ++Index)
	{
		if (ItemPickUpSlotGroup[Index]->IsActive()) continue;
		
		CopySlotCount = Index;
		break;
	}
	
	if (CopySlotCount > 0)
	{
		for (int Index = CopySlotCount; Index >= 1; --Index)
		{
			const TObjectPtr<UTwoMinWidget_ItemPickUpSlot> MoveSlot = ItemPickUpSlotGroup[Index];
			if (!MoveSlot) continue;
			
			const TObjectPtr<UTwoMinWidget_ItemPickUpSlot> PrevMoveSlot = ItemPickUpSlotGroup[Index - 1];
			if (!PrevMoveSlot) continue;
			
			FText Text = FText::FromString("");
			float PlayTime = 0;
			UTexture2D* Texture = nullptr;
			
			PrevMoveSlot->CopyItemSlotData(Text, PlayTime, Texture);
			MoveSlot->SetCopyItemSlotInfo(Text, PlayTime, Texture);
		}
	}
	else if (CopySlotCount == -1)
	{
		for (int Index = MaxItemPickUpSlotCount; Index >= 1; --Index)
		{
			const TObjectPtr<UTwoMinWidget_ItemPickUpSlot> MoveSlot = ItemPickUpSlotGroup[Index];
			if (!MoveSlot) continue;
			
			const TObjectPtr<UTwoMinWidget_ItemPickUpSlot> PrevMoveSlot = ItemPickUpSlotGroup[Index - 1];
			if (!PrevMoveSlot) continue;
			
			FText Text = FText::FromString("");
			float PlayTime = 0;
			UTexture2D* Texture = nullptr;
			
			PrevMoveSlot->CopyItemSlotData(Text, PlayTime, Texture);
			MoveSlot->SetCopyItemSlotInfo(Text, PlayTime, Texture);
		}
	}
	
	const TObjectPtr<UTwoMinWidget_ItemPickUpSlot> NewSlot = ItemPickUpSlotGroup[0];
	if (!NewSlot) return;
	
	NewSlot->SetItemSlotInfo(ItemName, ItemCount, ItemTexture);
}

void UTwoMinWidget_ItemPickUpWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!ItemPickUpList) return;
	
	TArray<UWidget*> ItemPickUpSlots = ItemPickUpList->GetAllChildren();
	if (ItemPickUpSlots.IsEmpty()) return;

	for (UWidget* ItemPickUpSlot : ItemPickUpSlots)
	{
		if (UTwoMinWidget_ItemPickUpSlot* CastSlot = Cast<UTwoMinWidget_ItemPickUpSlot>(ItemPickUpSlot))
		{
			ItemPickUpSlotGroup.Add(CastSlot);	
		}
	}
	
	if (ItemPickUpSlotGroup.IsEmpty()) return;

	for (const TObjectPtr<UTwoMinWidget_ItemPickUpSlot> TwoMinWidget_ItemPickUpSlot : ItemPickUpSlotGroup)
	{
		TwoMinWidget_ItemPickUpSlot->SetVisibility(ESlateVisibility::Hidden);
	}
	
	Algo::Reverse(ItemPickUpSlotGroup);
	MaxItemPickUpSlotCount = ItemPickUpSlotGroup.Num() - 1;
}
