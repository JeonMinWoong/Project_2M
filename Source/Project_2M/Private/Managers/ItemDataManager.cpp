// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/ItemDataManager.h"

#include "TwoMinFunctionLibrary.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/InventoryComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"

TMap<int32, int32> UItemDataManager::TryGetDropItems(const FString& DropTableOwner) const
{
	TMap<int32, int32> DropItems;
	TArray<FItemDropData*> DropTableGroup;
	
	if (DropTableOwner.IsEmpty())
	{
		return DropItems;
	}
	
	DropTable->GetAllRows(DropTableOwner, DropTableGroup);

	for (FItemDropData* TableItem : DropTableGroup)
	{
		if (TableItem && TableItem->ItemDropOwner == DropTableOwner)
		{
			int32 ItemCode = 0;
			int32 DropCount = 0;
			CalculateDropProbability(TableItem, ItemCode, DropCount);
			if (ItemCode == 0 || DropCount == 0) continue;
			if (DropItems.Contains(ItemCode)) continue;
			
			DropItems.Add(ItemCode, DropCount);
		}
	}
	
	return DropItems;
}

void UItemDataManager::GetDropItemList(TPair<int32, int32> Item, TArray<FItemEquipmentData>& OutItemEquipmentList,
	TArray<FItemConsumeData>& OutItemConsumeList, TArray<FItemEtcData>& OutItemEtcList) const
{
	EItemType ItemType = UTwoMinFunctionLibrary::GetItemType(Item.Key);
	if (ItemType == EItemType::Equipment)
	{
		FItemEquipmentData NewEquipmentData = GetItemEquipmentData(Item.Key);
		
		FItemEquipmentData ItemEquipmentData;
		ItemEquipmentData.ItemDataBase = NewEquipmentData.ItemDataBase;
		ItemEquipmentData.ItemDataBase.CurrentCount = Item.Value;
		if (NewEquipmentData.EquipmentPower.IsEmpty() == false)
		{
			for (auto Power : NewEquipmentData.EquipmentPower)
			{
				ItemEquipmentData.EquipmentPower.Add(Power);	
			}	
		}

		OutItemEquipmentList.Add(ItemEquipmentData);
	}
	else if (ItemType == EItemType::Consume)
	{
		FItemConsumeData NewConsumeData = GetItemConsumeData(Item.Key);
		
		FItemConsumeData ItemConsumeData;
		ItemConsumeData.ItemDataBase = NewConsumeData.ItemDataBase;
		ItemConsumeData.ItemDataBase.CurrentCount = Item.Value;
		if (NewConsumeData.ConsumePower.IsEmpty() == false)
		{
			for (auto Power : NewConsumeData.ConsumePower)
			{
				ItemConsumeData.ConsumePower.Add(Power);	
			}
		}
		
		OutItemConsumeList.Add(ItemConsumeData);
	}
	else if (ItemType == EItemType::Etc)
	{
		FItemEtcData NewEtcData = GetItemEtcData(Item.Key);
		
		FItemEtcData ItemEtcData;
		ItemEtcData.ItemDataBase = NewEtcData.ItemDataBase;
		ItemEtcData.ItemDataBase.CurrentCount = Item.Value;

		OutItemEtcList.Add(ItemEtcData);
	}
}

void UItemDataManager::GiveToInventoryBySingleItem(const ATwoMinPlayerCharacter* PlayerCharacter, const int32 ItemID, 
	const int32 ItemCount)
{
	TPair<int32, int32> GiveItem(ItemID, ItemCount);
	TArray<FItemEquipmentData> EquipmentList;
	TArray<FItemConsumeData> ConsumeList;
	TArray<FItemEtcData> EtcList;

	GetDropItemList(GiveItem, EquipmentList, ConsumeList, EtcList);
	GiveToInventory(PlayerCharacter, EquipmentList, ConsumeList, EtcList, true);
}

void UItemDataManager::GiveToInventory(const ATwoMinPlayerCharacter* PlayerCharacter, 
	TArray<FItemEquipmentData> InItemEquipmentList, TArray<FItemConsumeData> InItemConsumeList, 
	TArray<FItemEtcData> InItemEtcList, bool bIsNonPickUpWidget) const
{
	UInventoryComponent* Inventory = PlayerCharacter->GetInventoryComponent();
	int32 SaveAllItemCount = 0;
	for (const FItemEquipmentData& EquipmentList : InItemEquipmentList)
	{
		FItemEquipmentData NewEquipmentData = GetItemEquipmentData(EquipmentList.ItemDataBase.ItemID);
		Inventory->SaveToEquipmentInventory(EquipmentList, NewEquipmentData.ItemDataBase.ItemName, bIsNonPickUpWidget);	
		SaveAllItemCount++;
	}

	InItemEquipmentList.Empty();
	
	for (const FItemConsumeData& ConsumeList : InItemConsumeList)
	{
		FItemConsumeData NewConsumeData = GetItemConsumeData(ConsumeList.ItemDataBase.ItemID);
		Inventory->SaveToConsumeInventory(ConsumeList, NewConsumeData.ItemDataBase.ItemName, bIsNonPickUpWidget);
		SaveAllItemCount++;
	}

	InItemConsumeList.Empty();
	
	for (const FItemEtcData& EtcList : InItemEtcList)
	{
		FItemEtcData NewEtcData = GetItemEtcData(EtcList.ItemDataBase.ItemID);
		Inventory->SaveToEtcInventory(EtcList, NewEtcData.ItemDataBase.ItemName, bIsNonPickUpWidget);
		SaveAllItemCount++;
	}

	Inventory->UpdateInventory();
	
	if (bIsNonPickUpWidget == false)
	{
		Inventory->ShowPickUpGetItem(SaveAllItemCount);	
	}
	
	InItemEtcList.Empty();
}

FItemData UItemDataManager::GetItemDataBase(int32 ItemID) const
{
	EItemType ItemType = UTwoMinFunctionLibrary::GetItemType(ItemID);
	if (ItemType == EItemType::Equipment)
	{
		FItemEquipmentData NewEquipmentData = GetItemEquipmentData(ItemID);
		return NewEquipmentData.ItemDataBase;
	}
	
	if (ItemType == EItemType::Consume)
	{
		FItemConsumeData NewConsumeData = GetItemConsumeData(ItemID);
		return NewConsumeData.ItemDataBase;
	}
	
	if (ItemType == EItemType::Etc)
	{
		FItemEtcData NewEtcData = GetItemEtcData(ItemID);
		return NewEtcData.ItemDataBase;
	}
	
	return FItemData();
}

void UItemDataManager::CalculateDropProbability(const FItemDropData* ItemDropData, int32& OutItemCode,
                                                int32& OutDropCount) const
{
	const float RandValue = FMath::RandRange(0.f, 100.f);
	if (RandValue <= ItemDropData->Weight)
	{
		OutItemCode = ItemDropData->ItemID;
		OutDropCount = FMath::RandRange(ItemDropData->MinDropRange, ItemDropData->MaxDropRangeCount);
		return;
	}

	OutItemCode = 0;
	OutDropCount = 0;
}

FItemEquipmentData UItemDataManager::GetItemEquipmentData(int32 ItemID) const
{
	FItemEquipmentData NewEquipmentData;
	TArray<FItemEquipmentData*> EquipmentTableGroup;
	FString DropTableGroupName = FString::FromInt(ItemID);
	EquipmentDataTable->GetAllRows(DropTableGroupName, EquipmentTableGroup);

	for (FItemEquipmentData* TableGroup : EquipmentTableGroup)
	{
		if (ItemID == TableGroup->ItemDataBase.ItemID)
		{
			NewEquipmentData = *TableGroup;
			break;
		}
	}
	
	return NewEquipmentData;
}

FItemConsumeData UItemDataManager::GetItemConsumeData(int32 ItemID) const
{
	FItemConsumeData NewConsumeData;
	TArray<FItemConsumeData*> ConsumeTableGroup;
	FString DropTableGroupName = FString::FromInt(ItemID);
	ConsumeDataTable->GetAllRows(DropTableGroupName, ConsumeTableGroup);

	for (FItemConsumeData* TableGroup : ConsumeTableGroup)
	{
		if (ItemID == TableGroup->ItemDataBase.ItemID)
		{
			NewConsumeData = *TableGroup;
			break;
		}
	}
	
	return NewConsumeData;
}

FItemEtcData UItemDataManager::GetItemEtcData(int32 ItemID) const
{
	FItemEtcData NewEtcData;
	TArray<FItemEtcData*> EtcTableGroup;
	FString DropTableGroupName = FString::FromInt(ItemID);
	EtcDataTable->GetAllRows(DropTableGroupName, EtcTableGroup);

	for (FItemEtcData* TableGroup : EtcTableGroup)
	{
		if (ItemID == TableGroup->ItemDataBase.ItemID)
		{
			NewEtcData = *TableGroup;
			break;
		}
	}
	
	return NewEtcData;
}