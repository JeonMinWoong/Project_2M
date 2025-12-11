// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_QuickSlot.h"

#include "ToMinTypes/TwoMinEnumTypes.h"

UTwoMinWidget_QuickSlot::UTwoMinWidget_QuickSlot()
{
	InventorySlotType = EInventorySlotType::Quick;
}

void UTwoMinWidget_QuickSlot::SetInventorySlot(FItemInstance NewItemInstance)
{
	Super::SetInventorySlot(NewItemInstance);
	
	OnRegister(NewItemInstance.bIsRegister, NewItemInstance.SlotType, NewItemInstance.RegisterCount);
	SetRinkItemID(NewItemInstance.ItemID);
}

int32 UTwoMinWidget_QuickSlot::UnRegister()
{
	ClearSlot();
	int32 ResultItemID = RinkItemID;
	RinkItemID = 0;
	
	return ResultItemID;
}

void UTwoMinWidget_QuickSlot::SetRinkItemID(int32 NewRinkItemID)
{
	Super::SetRinkItemID(NewRinkItemID);
	
	RinkItemID = NewRinkItemID;
}
