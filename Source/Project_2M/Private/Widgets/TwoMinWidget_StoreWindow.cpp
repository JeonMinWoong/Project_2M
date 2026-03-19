// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_StoreWindow.h"

#include "Components/ListView.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridSlot.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/ItemDataManager.h"
#include "Widgets/TwoMinWidget_StoreSlot.h"

void UTwoMinWidget_StoreWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!StoreScrollBox) return;
	
	TArray<UWidget*> StoreGroup = StoreScrollBox->GetAllChildren();
	if (StoreGroup.IsEmpty()) return;

	for (UWidget* StoreSlot : StoreGroup)
	{
		UTwoMinWidget_StoreSlot* CastSlot = Cast<UTwoMinWidget_StoreSlot>(StoreSlot);
		if (!CastSlot) return;
		
		StoreSlots.Add(CastSlot);
	}
}

void UTwoMinWidget_StoreWindow::ResettingStoreList(const TSet<int32>& StoreList)
{
	const UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	for (auto StoreSlot : StoreSlots)
	{
		StoreSlot->ClearSlot();
	}
	
	TArray<int32> StoreArray = StoreList.Array();
	for (int32 Index = 0; Index < StoreArray.Num(); ++Index)
	{
		if (StoreSlots.IsValidIndex(Index) == false) break;
		if (StoreArray[Index] <= 0) continue;
		
		FItemData ItemData = GI->ItemDataManager->GetItemDataBase(StoreArray[Index]);
		if (ItemData.ItemID == 0) continue;
		
		StoreSlots[Index]->SetActiveSlot(ItemData);
	}

	for (int Index = 0; Index < FixedStoreSlotCount; ++Index)
	{
		StoreSlots[Index]->ShowStoreSlot(true);
	}
	
	for (int Index = 0; Index < StoreSlots.Num(); ++Index)
	{
		if (Index <= FixedStoreSlotCount - 1 || StoreSlots[Index]->GetItemID() != 0)
		{
			StoreSlots[Index]->ShowStoreSlot(true);
			continue;
		}
		
		StoreSlots[Index]->ShowStoreSlot(false);
	}
		
	CurStoreIndex = 0;
	MaxStoreIndex = StoreArray.Num() - 1;
}

void UTwoMinWidget_StoreWindow::SetFocusSlot(const int32 NewCurStoreIndex)
{
	if (StoreSlots.IsValidIndex(NewCurStoreIndex) == false) return;
	
	CurStoreIndex = NewCurStoreIndex;
	StoreSlots[CurStoreIndex]->SetFocus();
	
	int32 LastSlotIndex = StoreSlots.Num() - 1 > MaxStoreIndex ? MaxStoreIndex : StoreSlots.Num() - 1;
	if (NewCurStoreIndex == 0 || NewCurStoreIndex == LastSlotIndex)
	{
		if (NewCurStoreIndex == 0)
		{
			StoreScrollBox->ScrollToStart();	
		}
		else
		{
			StoreScrollBox->ScrollToEnd();	
		}
	}
	else
	{
		StoreScrollBox->ScrollWidgetIntoView(StoreSlots[CurStoreIndex], true);	
	}
}
