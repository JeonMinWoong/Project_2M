

#include "Compnents/InventoryComponent.h"

#include "MathUtil.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Controller/TwoMinPlayerController.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ItemDataManager.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "Widgets/TwoMinWidget_InventorySlot.h"
#include "Widgets/TwoMinWidget_InventoryUI.h"
#include "Widgets/TwoMinWidget_InventoryWindow.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!InventoryUIClass)
	{
		return;
	}
	
	if (!InventoryUI)
	{
		InventoryUI = CreateWidget<UTwoMinWidget_InventoryUI>(GetWorld(), InventoryUIClass);	
	}
	
	InitGiveItem();
}

void UInventoryComponent::InitGiveItem()
{
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	FItemEquipmentData BaseSwordData = GI->ItemDataManager->GetItemEquipmentData(10001);
	SaveToFinalInventory(BaseSwordData.ItemDataBase.ItemID, BaseSwordData.ItemDataBase.CurrentCount, 
		BaseSwordData.ItemDataBase.MaxCount, BaseSwordData.ItemDataBase.ItemTexture);
	
	FItemEquipmentData BaseShieldData = GI->ItemDataManager->GetItemEquipmentData(10002);
	SaveToFinalInventory(BaseShieldData.ItemDataBase.ItemID, BaseShieldData.ItemDataBase.CurrentCount, 
		BaseShieldData.ItemDataBase.MaxCount, BaseShieldData.ItemDataBase.ItemTexture);
	
	UpdateInventory();

	ForceEquipmentItem(BaseSwordData.ItemDataBase.ItemID, BaseSwordData.EquipmentType);
	ForceEquipmentItem(BaseShieldData.ItemDataBase.ItemID, BaseShieldData.EquipmentType);
}

void UInventoryComponent::OpenInventory(const bool bIsOpenInventory)
{
	FString Str = bIsOpenInventory ? TEXT("Open Inventory") : TEXT("Close Inventory");
	TwoMinDebugHelper::Print(Str, FColor::Green);
	
	if (!InventoryUI) return;
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwner());
	if (!PlayerCharacter) return;
	ATwoMinPlayerController* PC = PlayerCharacter->GetPlayerController();
	if (!PC) return;
	
	float TimeDilation;
	if (bIsOpenInventory)
	{
		TimeDilation = 0;
		InventoryUI->AddToViewport();
		InventoryUI->OnFocusSlot();
	}
	else
	{
		TimeDilation = 1.f;
		InventoryUI->RemoveFromParent();
	}
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

void UInventoryComponent::SaveToEquipmentInventory(const FItemEquipmentData& EquipmentData, const FString& ItemName)
{
	FItemData ItemBase = EquipmentData.ItemDataBase;
	int32 ExcessCount = SaveToFinalInventory(ItemBase.ItemID, ItemBase.CurrentCount,
		ItemBase.MaxCount, ItemBase.ItemTexture);
	SaveItemPickUpSlotData(ItemBase.ItemID, ItemName, ItemBase.CurrentCount - ExcessCount, ItemBase.ItemTexture);
}

void UInventoryComponent::SaveToConsumeInventory(const FItemConsumeData& ConsumeData, const FString& ItemName)
{
	FItemData ItemBase = ConsumeData.ItemDataBase;
	int32 ExcessCount =  SaveToFinalInventory(ItemBase.ItemID, ItemBase.CurrentCount, 
		ItemBase.MaxCount, ItemBase.ItemTexture);
	SaveItemPickUpSlotData(ItemBase.ItemID, ItemName, ItemBase.CurrentCount - ExcessCount, ItemBase.ItemTexture);
}

void UInventoryComponent::SaveToEtcInventory(const FItemEtcData& EtcData, const FString& ItemName)
{
	FItemData ItemBase = EtcData.ItemDataBase;
	int32 ExcessCount = SaveToFinalInventory(ItemBase.ItemID, ItemBase.CurrentCount,
		ItemBase.MaxCount, ItemBase.ItemTexture);
	SaveItemPickUpSlotData(ItemBase.ItemID, ItemName, ItemBase.CurrentCount - ExcessCount, ItemBase.ItemTexture);
}

int32 UInventoryComponent::SaveToFinalInventory(const int32 ItemID, const int32 ItemCount, const int32 ItemMaxCount, 
	UTexture2D* ItemTexture)
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
		ItemInstance.ItemTexture = ItemTexture;
		
		Inventory.Add(ItemInstance);

		FString Text = FString::Printf(TEXT("새로운 아이템 획득 : ItemID_%d, +%d"), ItemID, ItemCount);
		TwoMinDebugHelper::Print(Text, FColor::Green);
	}
	
	return OutExcessCount;
}


void UInventoryComponent::SaveItemPickUpSlotData(const int32 ItemId, const FString& ItemName, const int32 ItemCount, 
	UTexture2D* ItemTexture)
{
	FItemPickUpEntry ItemPickUpEntry;
	ItemPickUpEntry.ItemID = ItemId;
	ItemPickUpEntry.ItemName = ItemName;
	ItemPickUpEntry.ItemCount = ItemCount;
	ItemPickUpEntry.ItemTexture = ItemTexture;
	
	ItemPickUpSlotData.Add(ItemPickUpEntry);
}

void UInventoryComponent::ShowPickUpGetItem(const int32 SaveAllItemCount)
{
	if (const ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwner()))
	{
		PlayerCharacter->GetPlayerUIComponent()->OnItemPickUpSlot.Broadcast(SaveAllItemCount, ItemPickUpSlotData);
	}
	
	ItemPickUpSlotData.Empty();
}

void UInventoryComponent::UpdateInventory()
{
	if (!InventoryUI) return;
	
	UTwoMinWidget_EquipmentWindow* EquipmentWindow = InventoryUI->GetEquipmentWindow();
	UTwoMinWidget_InventoryWindow* InventoryWindow = InventoryUI->GetInventoryWindow();
	UTwoMinWidget_QuickWindow* QuickWindow = InventoryUI->GetQuickWindow();
	
	if (!InventoryWindow || !EquipmentWindow || !QuickWindow) return;
	
	Inventory.Sort([](const FItemInstance& A, const FItemInstance& B)
	{
		return A.ItemID < B.ItemID;
	});
	
	InventoryWindow->ClearAllSlots();
	EquipmentWindow->ClearAllSlots();
	QuickWindow->ClearAllSlots();
	
	for (auto NewItemInstance : Inventory)
	{
		for (auto InventorySlot : InventoryWindow->GetInventorySlots())
		{
			if (InventorySlot->GetItemInstance().ItemID != 0) continue;
			
			InventorySlot->SetInventorySlot(NewItemInstance);
			if (NewItemInstance.bIsRegister == false) break;
			
			InventorySlot->OnRegister(NewItemInstance.bIsRegister, NewItemInstance.SlotType, NewItemInstance.RegisterCount);
			if (NewItemInstance.SlotType == EInventorySlotType::Quick)
			{
				QuickWindow->GetInventorySlots()[NewItemInstance.RegisterCount]->SetInventorySlot(NewItemInstance);
			}
			else if (NewItemInstance.SlotType == EInventorySlotType::Equipment)
			{
				EquipmentWindow->GetInventorySlots()[NewItemInstance.RegisterCount]->SetInventorySlot(NewItemInstance);
			}
			
			break;
		}
	}
}

void UInventoryComponent::UseItem(int32 ItemID)
{
	bool bIsItemFind = false;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 Index = 0; Index < Inventory.Num(); Index++)
	{
		if (Inventory[Index].ItemID == ItemID)
		{
			bIsItemFind = true;
			Inventory[Index].HoldCount--;
			if (Inventory[Index].HoldCount <= 0)
			{
				RemoveIndex = Index;
			}
			
			break;
		}
	}
	
	if (bIsItemFind == false) return;
	
	if (RemoveIndex != INDEX_NONE)
	{
		Inventory.RemoveAt(RemoveIndex);
	}
	
	UpdateInventory();

	const UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	const FItemConsumeData ConsumeData = GI->ItemDataManager->GetItemConsumeData(ItemID);
	
	AActor* Owner = GetOwner();
	UConsumePayloadObject* ItemPayloadObject = NewObject<UConsumePayloadObject>(Owner);
	ItemPayloadObject->ConsumeData = ConsumeData;
	
	FGameplayEventData EventData;
	EventData.Instigator = Owner;
	EventData.OptionalObject = ItemPayloadObject;
					
	UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
		Owner, 
		TwoMinGameplayTag::Player_Event_UseItem, 
		EventData
	);
}

FItemInstance* UInventoryComponent::FindItemInstance(int32 ItemID)
{
	for (FItemInstance& Item : Inventory)
	{
		if (Item.ItemID == ItemID)
		{
			return &Item;
		}
	}
	
	return nullptr;
}

void UInventoryComponent::ForceEquipmentItem(int32 ItemID, EEquipmentType EquipmentType)
{
	UTwoMinWidget_EquipmentWindow* EquipmentWindow = InventoryUI->GetEquipmentWindow();
	UTwoMinWidget_InventoryWindow* InventoryWindow = InventoryUI->GetInventoryWindow();
	if (!InventoryWindow || !EquipmentWindow) return;
	
	FItemInstance* ItemInstance = FindItemInstance(ItemID);
	for (auto InventorySlot : InventoryWindow->GetInventorySlots())
	{
		if (InventorySlot->GetItemInstance().ItemID != ItemInstance->ItemID) continue;
		
		if (EquipmentType == EEquipmentType::Weapon_Right)
		{
			InventorySlot->OnRegister(true, EInventorySlotType::Equipment, 0);
			ItemInstance->OnRegister(true, EInventorySlotType::Equipment, 0);
			EquipmentWindow->GetInventorySlots()[0]->SetInventorySlot(*ItemInstance);
		}
		else if (EquipmentType == EEquipmentType::Weapon_Left)
		{
			InventorySlot->OnRegister(true, EInventorySlotType::Equipment, 1);
			ItemInstance->OnRegister(true, EInventorySlotType::Equipment, 1);
			EquipmentWindow->GetInventorySlots()[1]->SetInventorySlot(*ItemInstance);
		}
		
		break;
	}
}
