// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_EquipmentSlot.h"

#include "ToMinTypes/TwoMinEnumTypes.h"

UTwoMinWidget_EquipmentSlot::UTwoMinWidget_EquipmentSlot()
{
	InventorySlotType = EInventorySlotType::Equipment;
}

void UTwoMinWidget_EquipmentSlot::SetInventorySlot(FItemInstance NewItemInstance)
{
	Super::SetInventorySlot(NewItemInstance);
	
	OnRegister(NewItemInstance.bIsRegister, NewItemInstance.SlotType, NewItemInstance.RegisterCount);
	SetRinkItemID(NewItemInstance.ItemID);
}

int32 UTwoMinWidget_EquipmentSlot::UnRegister()
{
	ClearSlot();
	int32 ResultItemID = RinkItemID;
	RinkItemID = 0;
	
	return ResultItemID;
}

void UTwoMinWidget_EquipmentSlot::SetRinkItemID(int32 NewRinkItemID)
{
	Super::SetRinkItemID(NewRinkItemID);
	
	RinkItemID = NewRinkItemID;
}
