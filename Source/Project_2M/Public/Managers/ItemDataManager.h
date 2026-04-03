// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "ItemDataManager.generated.h"

class ATwoMinPlayerCharacter;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class PROJECT_2M_API UItemDataManager : public UObject
{
	GENERATED_BODY()
	
public:
	void GiveToInventoryBySingleItem(const ATwoMinPlayerCharacter* PlayerCharacter, const int32 ItemID, const int32 ItemCount);
	
	TMap<int32, int32> TryGetDropItems(const FString& DropTableOwner) const;
	void GetDropItemList(TPair<int32, int32> Item, TArray<FItemEquipmentData>& OutItemEquipmentList, 
		TArray<FItemConsumeData>& OutItemConsumeList, TArray<FItemEtcData>& OutItemEtcList) const;
	void GiveToInventory(const ATwoMinPlayerCharacter* PlayerCharacter, 
	                     TArray<FItemEquipmentData> InItemEquipmentList, TArray<FItemConsumeData> InItemConsumeList,
	                     TArray<FItemEtcData> InItemEtcList, bool bIsNonPickUpWidget) const;
	
	FItemData GetItemDataBase(int32 ItemID) const;
	FItemEquipmentData GetItemEquipmentData(int32 ItemID) const;
	FItemConsumeData GetItemConsumeData(int32 ItemID) const;
	FItemEtcData GetItemEtcData(int32 ItemID) const;

private:
	void CalculateDropProbability(const FItemDropData* ItemDropData, int32& OutItemCode, int32& OutDropCount) const;
	void CacheAllItemData();

	UPROPERTY(EditDefaultsOnly, Category="EquipmentData")
	UDataTable* EquipmentDataTable;

	UPROPERTY(EditDefaultsOnly, Category="ConsumeData")
	UDataTable* ConsumeDataTable;

	UPROPERTY(EditDefaultsOnly, Category="EtcData")
	UDataTable* EtcDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "StageInfo|DropTable")
	UDataTable* DropTable;

	bool bIsCached = false;
	TMap<int32, FItemEquipmentData> CachedEquipmentData;
	TMap<int32, FItemConsumeData> CachedConsumeData;
	TMap<int32, FItemEtcData> CachedEtcData;
};
