

#include "Compnents/InventoryComponent.h"

#include "MathUtil.h"
#include "TwoMinDebugHelper.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UInventoryComponent::SaveToEquipmentInventory(const FItemEquipmentData& EquipmentData, const FString& ItemName)
{
	int32 ExcessCount = SaveToFinalInventory(EquipmentData.ItemDataBase.ItemID, 
		EquipmentData.ItemDataBase.CurrentCount, EquipmentData.ItemDataBase.MaxCount);
	SaveItemPickUpSlotData(ItemName, EquipmentData.ItemDataBase.CurrentCount - ExcessCount,
		EquipmentData.ItemDataBase.ItemTexture);
}

void UInventoryComponent::SaveToConsumeInventory(const FItemConsumeData& ConsumeData, const FString& ItemName)
{
	int32 ExcessCount =  SaveToFinalInventory(ConsumeData.ItemDataBase.ItemID, 
		ConsumeData.ItemDataBase.CurrentCount, ConsumeData.ItemDataBase.MaxCount);
	SaveItemPickUpSlotData(ItemName, ConsumeData.ItemDataBase.CurrentCount - ExcessCount, 
		ConsumeData.ItemDataBase.ItemTexture);
}

void UInventoryComponent::SaveToEtcInventory(const FItemEtcData& EtcData, const FString& ItemName)
{
	int32 ExcessCount = SaveToFinalInventory(EtcData.ItemDataBase.ItemID, 
		EtcData.ItemDataBase.CurrentCount, EtcData.ItemDataBase.MaxCount);
	SaveItemPickUpSlotData(ItemName, EtcData.ItemDataBase.CurrentCount - ExcessCount, EtcData.ItemDataBase.ItemTexture);
}

int32 UInventoryComponent::SaveToFinalInventory(const int32 ItemID, const int32 ItemCount, const int32 ItemMaxCount)
{
	int32 OutExcessCount = 0;
	FItemInstance* OldItemInstance = nullptr;
	for (FItemInstance& ItemInstance : Inventory)
	{
		if (ItemInstance.ItemID == ItemID)
		{
			OldItemInstance = &ItemInstance;
			break;
		}
	}
	
	if (OldItemInstance)
	{
		OutExcessCount = FMathf::Max(0 ,OldItemInstance->HoldCount + ItemCount - ItemMaxCount);
		if (OutExcessCount > 0)
		{
			OldItemInstance->HoldCount = FMathf::Min(ItemMaxCount, OldItemInstance->HoldCount + ItemCount);
			FString AfterText = FString::Printf(TEXT("기존 아이템 초과 : ItemID_%d, +%d"), ItemID, OutExcessCount);
			TwoMinDebugHelper::Print(AfterText, FColor::Red);
			return OutExcessCount;
		}
		
		OldItemInstance->HoldCount += ItemCount;

		FString AfterText = FString::Printf(TEXT("기존 아이템 중첩 : ItemID_%d, +%d"), ItemID, ItemCount);
		TwoMinDebugHelper::Print(AfterText, FColor::Yellow);
	}
	else
	{
		FItemInstance ItemInstance;
		ItemInstance.ItemID = ItemID;
		ItemInstance.HoldCount = ItemCount;
		
		Inventory.Add(ItemInstance);

		FString Text = FString::Printf(TEXT("새로운 아이템 획득 : ItemID_%d, +%d"), ItemID, ItemCount);
		TwoMinDebugHelper::Print(Text, FColor::Green);
	}
	
	return OutExcessCount;
}


void UInventoryComponent::SaveItemPickUpSlotData(const FString& ItemName, const int32 ItemCount, UTexture2D* ItemTexture)
{
	FItemPickUpEntry ItemPickUpEntry;
	ItemPickUpEntry.ItemName = ItemName;
	ItemPickUpEntry.ItemCount = ItemCount;
	ItemPickUpEntry.ItemTexture = ItemTexture;
	
	ItemPickUpSlotData.Add(ItemPickUpEntry);
}

void UInventoryComponent::ShowAllItem(int32 SaveAllItemCount)
{
	if (const ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwner()))
	{
		PlayerCharacter->GetPlayerUIComponent()->OnItemPickUpSlot.Broadcast(SaveAllItemCount, ItemPickUpSlotData);
	}
	
	ItemPickUpSlotData.Empty();
}
