// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidget_InventorySlot.h"
#include "TwoMinWidget_EquipmentSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_EquipmentSlot : public UTwoMinWidget_InventorySlot
{
	GENERATED_BODY()
	
public:
	UTwoMinWidget_EquipmentSlot();
	
	virtual void SetInventorySlot(FItemInstance NewItemInstance) override;
	virtual int32 UnRegister() override;
	virtual void SetRinkItemID(int32 NewRinkItemID) override;

private:
	UPROPERTY()
	int32 RinkItemID;
	
};
