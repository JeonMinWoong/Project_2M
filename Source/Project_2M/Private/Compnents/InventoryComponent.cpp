

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
#include "Widgets/TwoMinWidget_InventorySlot.h"
#include "Widgets/TwoMinWidget_InventoryUI.h"
#include "Widgets/TwoMinWidget_InventoryWindow.h"
#include "Widgets/TwoMinWidget_StoreUI.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitStoreInventoryWindow();
	InitGiveItem();
}

void UInventoryComponent::InitStoreInventoryWindow()
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwner());
	
	if (!PlayerCharacter) return;
	StoreInventoryWindow = PlayerCharacter->GetPlayerUIComponent()->GetStoreUI()->GetInventoryWindow();
}

void UInventoryComponent::InitGiveItem()
{
	if (!InventoryUIClass)
	{
		return;
	}
	
	if (!InventoryUI)
	{
		InventoryUI = CreateWidget<UTwoMinWidget_InventoryUI>(GetWorld(), InventoryUIClass);	
	}
	
	if (UTwoMinFunctionLibrary::IsLoadingData(this) == false)
	{
		// 처음 아이템 추가.
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
	else
	{
		UpdateInventory();
		
		// 로드 아이템 장착.
		UTwoMinWidget_EquipmentWindow* EquipmentWindow = InventoryUI->GetEquipmentWindow();
		UTwoMinWidget_QuickWindow* QuickWindow = InventoryUI->GetQuickWindow();
		UTwoMinWidget_InventoryWindow* InventoryWindow = InventoryUI->GetInventoryWindow();
		if (!InventoryWindow || !QuickWindow || !EquipmentWindow) return;
		
		UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
		for (auto InventorySlot :  InventoryWindow->GetInventorySlots())
		{
			int32 SlotItemID = InventorySlot->GetItemInstance().ItemID;
			
			FItemInstance* ItemInstance = FindItemInstance(SlotItemID);
			if (!ItemInstance) continue;
			if (ItemInstance->bIsRegister == false) continue;
			
			EItemType ItemType = UTwoMinFunctionLibrary::GetItemType(SlotItemID);
			
			if (ItemType == EItemType::Etc) continue;
			
			if (ItemType == EItemType::Equipment)
			{
				FItemEquipmentData EquipmentData = GI->ItemDataManager->GetItemEquipmentData(SlotItemID);
				if (EquipmentData.EquipmentType == EEquipmentType::Weapon_Right)
				{
					InventorySlot->OnRegister(true, EInventorySlotType::Equipment, 0);
					ItemInstance->OnRegister(true, EInventorySlotType::Equipment, 0);
					EquipmentWindow->GetInventorySlots()[0]->SetInventorySlot(*ItemInstance);
					InventoryUI->OnEquipment(ItemInstance->ItemID);
				}
				else if (EquipmentData.EquipmentType == EEquipmentType::Weapon_Left)
				{
					InventorySlot->OnRegister(true, EInventorySlotType::Equipment, 1);
					ItemInstance->OnRegister(true, EInventorySlotType::Equipment, 1);
					EquipmentWindow->GetInventorySlots()[1]->SetInventorySlot(*ItemInstance);
					InventoryUI->OnEquipment(ItemInstance->ItemID);
				}	
			}
			else if (ItemType == EItemType::Consume)
			{
				int32 RegisterCount = ItemInstance->RegisterCount;
				InventorySlot->OnRegister(true, EInventorySlotType::Quick, RegisterCount);
				ItemInstance->OnRegister(true, EInventorySlotType::Quick, RegisterCount);
				QuickWindow->GetInventorySlots()[RegisterCount]->SetInventorySlot(*ItemInstance);
			}
		}
	}
}

void UInventoryComponent::OpenInventory(const bool bIsOpenInventory)
{
	if (!InventoryUI) return;
	
	float TimeDilation;
	if (bIsOpenInventory)
	{
		TimeDilation = 0.0001f;
		InventoryUI->AddToViewport();
		InventoryUI->OnFocusSlot();
		UpdateStatus();
	}
	else
	{
		TimeDilation = 1.f;
		InventoryUI->ResetInventoryUI();
		InventoryUI->RemoveFromParent();
	}
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

void UInventoryComponent::SaveToEquipmentInventory(const FItemEquipmentData& EquipmentData, const FString& ItemName, 
	bool bIsNonPickUpWidget)
{
	FItemData ItemBase = EquipmentData.ItemDataBase;
	int32 ExcessCount = SaveToFinalInventory(ItemBase.ItemID, ItemBase.CurrentCount,
		ItemBase.MaxCount, ItemBase.ItemTexture);
	if (bIsNonPickUpWidget == false)
	{
		SaveItemPickUpSlotData(ItemBase.ItemID, ItemName, ItemBase.CurrentCount - ExcessCount, ItemBase.ItemTexture);
	}
}

void UInventoryComponent::SaveToConsumeInventory(const FItemConsumeData& ConsumeData, const FString& ItemName, 
	bool bIsClearStage)
{
	FItemData ItemBase = ConsumeData.ItemDataBase;
	int32 ExcessCount =  SaveToFinalInventory(ItemBase.ItemID, ItemBase.CurrentCount, 
		ItemBase.MaxCount, ItemBase.ItemTexture);
	if (bIsClearStage == false)
	{
		SaveItemPickUpSlotData(ItemBase.ItemID, ItemName, ItemBase.CurrentCount - ExcessCount, ItemBase.ItemTexture);	
	}
}

void UInventoryComponent::SaveToEtcInventory(const FItemEtcData& EtcData, const FString& ItemName, bool bIsClearStage)
{
	FItemData ItemBase = EtcData.ItemDataBase;
	int32 ExcessCount = SaveToFinalInventory(ItemBase.ItemID, ItemBase.CurrentCount,
		ItemBase.MaxCount, ItemBase.ItemTexture);
	if (bIsClearStage == false)
	{
		SaveItemPickUpSlotData(ItemBase.ItemID, ItemName, ItemBase.CurrentCount - ExcessCount, ItemBase.ItemTexture);	
	}
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
			//FString AfterText = FString::Printf(TEXT("기존 아이템 초과 : ItemID_%d, +%d"), ItemID, OutExcessCount);
			//TwoMinDebugHelper::Print(AfterText, FColor::Red);
			return OutExcessCount;
		}
		
		OldItemInstance->HoldCount += ItemCount;

		//FString AfterText = FString::Printf(TEXT("기존 아이템 중첩 : ItemID_%d, +%d"), ItemID, ItemCount);
		//TwoMinDebugHelper::Print(AfterText, FColor::Yellow);
	}
	else
	{
		FItemInstance ItemInstance;
		ItemInstance.ItemID = ItemID;
		ItemInstance.HoldCount = ItemCount;
		ItemInstance.ItemTexture = ItemTexture;
		
		Inventory.Add(ItemInstance);

		//FString Text = FString::Printf(TEXT("새로운 아이템 획득 : ItemID_%d, +%d"), ItemID, ItemCount);
		//TwoMinDebugHelper::Print(Text, FColor::Green);
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
	
	if (!InventoryWindow || !EquipmentWindow || !QuickWindow || !StoreInventoryWindow) return;
	
	Inventory.Sort([](const FItemInstance& A, const FItemInstance& B)
	{
		return A.ItemID < B.ItemID;
	});
	
	InventoryWindow->ClearAllSlots();
	EquipmentWindow->ClearAllSlots();
	QuickWindow->ClearAllSlots();
	
	StoreInventoryWindow->ClearAllSlots();
	
	TArray<TObjectPtr<UTwoMinWidget_InventorySlot>> InventorySlots = InventoryWindow->GetInventorySlots();
	TArray<TObjectPtr<UTwoMinWidget_InventorySlot>> StoreInventorySlots = StoreInventoryWindow->GetInventorySlots();
	for (auto NewItemInstance : Inventory)
	{
		for (int32 Index = 0; Index < InventorySlots.Num(); ++Index)
		{
			if (InventorySlots[Index]->GetItemInstance().ItemID != 0) continue;
			
			InventorySlots[Index]->SetInventorySlot(NewItemInstance);
			StoreInventorySlots[Index]->SetInventorySlot(NewItemInstance);
			
			if (NewItemInstance.bIsRegister == false) break;
			
			InventorySlots[Index]->OnRegister(NewItemInstance.bIsRegister, NewItemInstance.SlotType, NewItemInstance.RegisterCount);
			StoreInventorySlots[Index]->OnRegister(NewItemInstance.bIsRegister, NewItemInstance.SlotType, NewItemInstance.RegisterCount);
			
			if (NewItemInstance.SlotType == EInventorySlotType::Quick)
			{
				QuickWindow->GetInventorySlots()[NewItemInstance.RegisterCount]->SetInventorySlot(NewItemInstance);
				ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwner());
				UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetPlayerUIComponent();
				PlayerUIComponent->OnSetWindowQuickSlot.Broadcast(NewItemInstance, NewItemInstance.RegisterCount, true);
			}
			else if (NewItemInstance.SlotType == EInventorySlotType::Equipment)
			{
				EquipmentWindow->GetInventorySlots()[NewItemInstance.RegisterCount]->SetInventorySlot(NewItemInstance);
			}
			
			break;
		}
	}
}

void UInventoryComponent::UseItem(int32 ItemID, bool& bIsRemoved)
{
	bIsRemoved = false;
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
		bIsRemoved = true;
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
	
	// todo : 인벤 연동 작업 해야힘.
	TArray<TObjectPtr<UTwoMinWidget_InventorySlot>> InventorySlots = InventoryWindow->GetInventorySlots();
	TArray<TObjectPtr<UTwoMinWidget_InventorySlot>> StoreInventorySlots = StoreInventoryWindow->GetInventorySlots();
	
	FItemInstance* ItemInstance = FindItemInstance(ItemID);
	for (int32 Index = 0; Index < InventorySlots.Num(); ++Index)
	{
		if (InventorySlots[Index]->GetItemInstance().ItemID != ItemInstance->ItemID) continue;
		
		if (EquipmentType == EEquipmentType::Weapon_Right)
		{
			InventorySlots[Index]->OnRegister(true, EInventorySlotType::Equipment, 0);
			StoreInventorySlots[Index]->OnRegister(true, EInventorySlotType::Equipment, 0);
			ItemInstance->OnRegister(true, EInventorySlotType::Equipment, 0);
			EquipmentWindow->GetInventorySlots()[0]->SetInventorySlot(*ItemInstance);
			InventoryUI->OnEquipment(ItemInstance->ItemID);
		}
		else if (EquipmentType == EEquipmentType::Weapon_Left)
		{
			InventorySlots[Index]->OnRegister(true, EInventorySlotType::Equipment, 1);
			StoreInventorySlots[Index]->OnRegister(true, EInventorySlotType::Equipment, 1);
			ItemInstance->OnRegister(true, EInventorySlotType::Equipment, 1);
			EquipmentWindow->GetInventorySlots()[1]->SetInventorySlot(*ItemInstance);
			InventoryUI->OnEquipment(ItemInstance->ItemID);
		}
		
		break;
	}
}

FItemInstance UInventoryComponent::GetQuickSlotItemInstance(int32 SlotIndex) const
{
	UTwoMinWidget_QuickWindow* QuickWindow = InventoryUI->GetQuickWindow();
	if (!QuickWindow) return FItemInstance();
	FItemInstance QuickSlotItem = QuickWindow->GetInventorySlots()[SlotIndex]->GetItemInstance();
	return QuickSlotItem;
}

void UInventoryComponent::UpdateStatus()
{
	UTwoMinWidget_EquipmentWindow* EquipmentWindow = InventoryUI->GetEquipmentWindow();
	if (!EquipmentWindow) return;
	
	EquipmentWindow->UpdateStatusText();
}

void UInventoryComponent::SellInventoryItem(const int32 ItemID, const int32 SellCount)
{
	bool bIsItemFind = false;
	int32 RemoveIndex = INDEX_NONE;
	
	for (int32 Index = 0; Index < Inventory.Num(); Index++)
	{
		if (Inventory[Index].ItemID == ItemID)
		{
			bIsItemFind = true;
			Inventory[Index].HoldCount -= SellCount;
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
	
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	if (!GI) return;
	
	FItemData CurItemData = GI->ItemDataManager->GetItemDataBase(ItemID);
	if (CurItemData.ItemID == 0) return;
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwner());
	if (!PlayerCharacter) return;
	
	UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;
	
	const int32 TotalSellPrice = CurItemData.SellPrice * SellCount;
	ASC->GiveGoldAmount(TotalSellPrice);
}

void UInventoryComponent::BuyInventoryItem(const int32 ItemID, const int32 BuyCount)
{
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	if (!GI) return;
	
	FItemData CurItemData = GI->ItemDataManager->GetItemDataBase(ItemID);
	if (CurItemData.ItemID == 0) return;
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwner());
	if (!PlayerCharacter) return;
	
	UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;
	
	const int32 TotalBuyPrice = -CurItemData.BuyPrice * BuyCount;
	ASC->GiveGoldAmount(TotalBuyPrice);
	GI->ItemDataManager->GiveToInventoryBySingleItem(PlayerCharacter, ItemID, BuyCount);
}
