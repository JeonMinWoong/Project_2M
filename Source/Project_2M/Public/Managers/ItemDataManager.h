// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "ItemDataManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class PROJECT_2M_API UItemDataManager : public UObject
{
	GENERATED_BODY()
	
public:
	FItemEquipmentData GetItemEquipmentData(int32 ItemID) const;
	FItemConsumeData GetItemConsumeData(int32 ItemID) const;
	FItemEtcData GetItemEtcData(int32 ItemID) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="EquipmentData")
	UDataTable* EquipmentDataTable;

	UPROPERTY(EditDefaultsOnly, Category="ConsumeData")
	UDataTable* ConsumeDataTable;

	UPROPERTY(EditDefaultsOnly, Category="EtcData")
	UDataTable* EtcDataTable;
};
