// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_StoreWindow.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/ItemDataManager.h"
#include "Widgets/TwoMinWidget_StoreSlot.h"

void UTwoMinWidget_StoreWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!StoreGrid) return;
	
	TArray<UWidget*> StoreGroup = StoreGrid->GetAllChildren();
	if (StoreGroup.IsEmpty()) return;

	for (UWidget* StoreSlot : StoreGroup)
	{
		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(StoreSlot->Slot);
		if (!GridSlot) continue;
		
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
	
		
	CurStoreIndex = 0;
	MaxStoreIndex = StoreArray.Num() - 1;
}

void UTwoMinWidget_StoreWindow::SetFocusSlot(const int32 NewCurStoreIndex)
{
	if (StoreSlots.IsValidIndex(NewCurStoreIndex) == false) return;
	
	CurStoreIndex = NewCurStoreIndex;
	StoreSlots[CurStoreIndex]->SetFocus();
}
