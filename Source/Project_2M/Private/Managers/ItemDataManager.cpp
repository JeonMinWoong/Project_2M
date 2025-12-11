// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/ItemDataManager.h"

#include "ToMinTypes/TwoMinStructTypes.h"

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