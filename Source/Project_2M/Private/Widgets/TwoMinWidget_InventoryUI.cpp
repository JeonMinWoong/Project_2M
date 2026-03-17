// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_InventoryUI.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility_WeaponSpawn.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/InventoryComponent.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "GameModes/TwoMinBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ItemDataManager.h"
#include "Managers/WorldStageManager.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidget_InventorySlot.h"
#include "Widgets/TwoMinWidget_InventoryWindow.h"
#include "Widgets/TwoMinWidget_ItemInfoPopup.h"

UTwoMinWidget_InventoryUI::UTwoMinWidget_InventoryUI()
{
	CurInventoryWindowType = EInventoryWindowType::Inventory;
}

void UTwoMinWidget_InventoryUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	InventorySelect->SetVisibility(ESlateVisibility::Hidden);

	const bool bIsUsingGamePad =
		UTwoMinFunctionLibrary::IsUsingGamePad(GetWorld(), GetOwningPlayer()->GetPlatformUserId());
	
	KeyBoardBox->SetVisibility(bIsUsingGamePad ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	GamePadBox->SetVisibility(bIsUsingGamePad ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
	const bool bIsVillageMap = UTwoMinFunctionLibrary::IsVillageMap(GetWorld());
	const FString Str = FString::Printf(bIsVillageMap ? TEXT(": 수동 저장") : TEXT(": 전투 포기"));
	AdditionalButtonText->SetText(FText::FromString(Str));
}

FReply UTwoMinWidget_InventoryUI::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	TArray<UTwoMinWidget_InventorySlot*> EquipmentSlots = EquipmentWindow->GetInventorySlots();
	TArray<UTwoMinWidget_InventorySlot*> QuickSlots = QuickWindow->GetInventorySlots();
	TArray<UTwoMinWidget_InventorySlot*> InventorySlots = InventoryWindow->GetInventorySlots();
	
	if (InventorySlots.IsEmpty() || EquipmentSlots.IsEmpty() || QuickSlots.IsEmpty()) return FReply::Unhandled();

	int32 CurInventoryIndex = 0;
	int32 MaxInventoryIndex = 0;
	int32 MaxColumnIndex = 0;
	
	if (CurInventoryWindowType == EInventoryWindowType::Inventory)
	{
		CurInventoryIndex = InventoryWindow->GetCurInventoryIndex();
		MaxInventoryIndex = InventoryWindow->GetMaxInventoryIndex();
		MaxColumnIndex = InventoryWindow->GetMaxColumnIndex();	
	}
	else if (CurInventoryWindowType == EInventoryWindowType::Equipment)
	{
		CurInventoryIndex = EquipmentWindow->GetCurInventoryIndex();
		MaxInventoryIndex = EquipmentWindow->GetMaxInventoryIndex();
		MaxColumnIndex = EquipmentWindow->GetMaxColumnIndex();
	}
	else
	{
		CurInventoryIndex = QuickWindow->GetCurInventoryIndex();
		MaxInventoryIndex = QuickWindow->GetMaxInventoryIndex();
		MaxColumnIndex = QuickWindow->GetMaxColumnIndex();
	}
	
	bool bIsSelectOpen = InventorySelect->IsOpen();
	bool bIsPopupOpen = ItemInfoPopup->IsPopupOpen();
	const FKey InKey = InKeyEvent.GetKey();
	
	if (InKey == EKeys::R || InKey == EKeys::Gamepad_FaceButton_Top)
	{
		ATwoMinBaseGameMode* GM = Cast<ATwoMinBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (!GM) return FReply::Unhandled();
		
		if (UTwoMinFunctionLibrary::IsVillageMap(GetWorld()))
		{
			if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn()))
			{
				PlayerCharacter->OpenInventoryProcess();
				PlayerCharacter->GetPlayerUIComponent()->OnStartManualSaveAnim.Broadcast();
			}
			
			FSaveGameData NewSaveGameData;
			GM->CreateNewSaveGameData(NewSaveGameData);
	
			UTwoMinFunctionLibrary::SaveGame(NewSaveGameData);
			return FReply::Handled();
		}
		else
		{
			UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
			if (!GI) return FReply::Unhandled();

			FString CurRealStageName = GetWorld()->RemovePIEPrefix(GetWorld()->GetMapName());

			const int32 CurStageIndex = GI->StateManager->GetWorldStageIndex(CurRealStageName);
			FName GoStageName = FName(*GI->StateManager->GetIndexRealStageName(0));
			if (CurStageIndex != 0)
			{
				GoStageName = FName(*GI->StateManager->GetVillageName());
			}

			GM->OpenStageProcess(GoStageName, false);
			return FReply::Handled();
		}
	}
	
	if (InKey == EKeys::Enter || InKey == EKeys::F || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
		if (bIsPopupOpen)
		{
			HideItemInfoPopup();
			return FReply::Handled();
		}
		
		if (bIsSelectOpen)
		{
			if (bIsQuickRegister)
			{
				bIsQuickRegister = false;
				
				HideInventorySelect();

				int32 SelectInventoryIndex = InventoryWindow->GetCurInventoryIndex();
				FItemInstance ItemInstance = InventorySlots[SelectInventoryIndex]->GetItemInstance();
				FItemInstance* InventoryItem = FindInventoryItem(ItemInstance.ItemID);
				
				if (ItemInstance.bIsRegister)
				{
					if (UTwoMinWidget_InventorySlot* QuickSlot = FindQuickSlot(ItemInstance.ItemID))
					{
						QuickSlot->UnRegister();
					}
					
					// Overlay 먼저 해제
					if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn()))
					{
						UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetPlayerUIComponent();
						PlayerUIComponent->OnSetWindowQuickSlot.Broadcast(ItemInstance, ItemInstance.RegisterCount, false);
					}
				}
				
				int32 CurQuickIndex = QuickWindow->GetCurInventoryIndex();
				if (QuickSlots[CurQuickIndex]->IsRegister())
				{
					int32 RinkItemID = QuickSlots[CurQuickIndex]->UnRegister();
					UTwoMinWidget_InventorySlot* RinkInventorySlot = FindInventorySlot(RinkItemID);
					RinkInventorySlot->UnRegister();
					FItemInstance* RinkInventoryItem = FindInventoryItem(RinkItemID);
					RinkInventoryItem->UnRegister();
				}
				
				//InventorySlots[SelectInventoryIndex]->OnRegister(true, EInventorySlotType::Quick, CurQuickIndex);
				InventoryItem->OnRegister(true, EInventorySlotType::Quick, CurQuickIndex);
				QuickSlots[CurQuickIndex]->SetInventorySlot(*InventoryItem);
				
				ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
				if (!PlayerCharacter) return FReply::Unhandled();
				
				UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetPlayerUIComponent();
				if (!PlayerUIComponent) return FReply::Unhandled();
				
				PlayerCharacter->GetInventoryComponent()->UpdateInventory();
				PlayerUIComponent->OnSetWindowQuickSlot.Broadcast(*InventoryItem, CurQuickIndex, true);
				
				return FReply::Handled();
			}
			
			int32 CurIndex = InventorySelect->GetInventorySelectIndex();
			ESelectEventType SelectEvent = InventorySelect->OnTriggerEvent(CurIndex);
			if (SelectEvent == ESelectEventType::Cancel)
			{
				HideInventorySelect();
				
				return FReply::Handled();
			}
			else if (SelectEvent == ESelectEventType::InventoryToConsume_Register)
			{
				bIsQuickRegister = true;
				
				CurInventoryWindowType = EInventoryWindowType::Quick;
				int32 NextInventoryIndex = 0;
				MoveToQuickSlot(QuickSlots[NextInventoryIndex], NextInventoryIndex);
				
				return FReply::Handled();
			}
			else if (SelectEvent == ESelectEventType::InventoryToConsume_Use)
			{
				HideInventorySelect();
				if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn()))
				{
					int32 SelectInventoryIndex = InventoryWindow->GetCurInventoryIndex();
					FItemInstance ItemInstance = InventorySlots[SelectInventoryIndex]->GetItemInstance();
					
					bool bIsRemoved = false;
					PlayerCharacter->OpenInventoryProcess();
					PlayerCharacter->GetInventoryComponent()->UseItem(ItemInstance.ItemID, bIsRemoved);
					
					if (ItemInstance.bIsRegister && bIsRemoved)
					{
						UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetPlayerUIComponent();
						PlayerUIComponent->OnSetWindowQuickSlot.Broadcast(ItemInstance, ItemInstance.RegisterCount, false);
					}
				}
				
				return FReply::Handled();
			}
			else if (SelectEvent == ESelectEventType::QuickToConsume_NonRegister)
			{
				HideInventorySelect();
				
				int32 SelectQuickIndex = QuickWindow->GetCurInventoryIndex();
				FItemInstance ItemInstance = QuickSlots[SelectQuickIndex]->GetItemInstance();
				
				// Overlay 먼저 해제
				if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn()))
				{
					UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetPlayerUIComponent();
					PlayerUIComponent->OnSetWindowQuickSlot.Broadcast(ItemInstance, ItemInstance.RegisterCount, false);
				}
				
				int32 RinkItemID = QuickSlots[SelectQuickIndex]->UnRegister();
				UTwoMinWidget_InventorySlot* RinkInventorySlot = FindInventorySlot(RinkItemID);
				RinkInventorySlot->UnRegister();
				FItemInstance* InventoryItem = FindInventoryItem(ItemInstance.ItemID);
				InventoryItem->UnRegister();
				
				return FReply::Handled();
			}
			else if (SelectEvent == ESelectEventType::QuickToConsume_Use)
			{
				HideInventorySelect();
				
				if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn()))
				{
					int32 SelectQuickIndex = QuickWindow->GetCurInventoryIndex();
					FItemInstance ItemInstance = QuickSlots[SelectQuickIndex]->GetItemInstance();
					
					bool bIsRemoved = false;
					PlayerCharacter->OpenInventoryProcess();
					PlayerCharacter->GetInventoryComponent()->UseItem(ItemInstance.ItemID, bIsRemoved);
					
					if (ItemInstance.bIsRegister && bIsRemoved)
					{
						UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetPlayerUIComponent();
						PlayerUIComponent->OnSetWindowQuickSlot.Broadcast(ItemInstance, ItemInstance.RegisterCount, false);
					}
				}
				
				return FReply::Handled();
			}
			else if (SelectEvent == ESelectEventType::InventoryToEquipment_Change)
			{
				HideInventorySelect();

				int32 EquipmentIndex = 0;
				int32 SelectInventoryIndex = InventoryWindow->GetCurInventoryIndex();
				FItemInstance ItemInstance = InventorySlots[SelectInventoryIndex]->GetItemInstance();
				FItemInstance* InventoryItem = FindInventoryItem(ItemInstance.ItemID);
				
				UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
				FItemEquipmentData ItemData = GI->ItemDataManager->GetItemEquipmentData(ItemInstance.ItemID);
				if (ItemData.EquipmentType == EEquipmentType::Weapon_Right)
				{
					EquipmentIndex = 0;
				}
				else if (ItemData.EquipmentType == EEquipmentType::Weapon_Left)
				{
					EquipmentIndex = 1;
				}
				
				if (EquipmentSlots[EquipmentIndex]->IsRegister())
				{
					int32 RinkItemID = EquipmentSlots[EquipmentIndex]->UnRegister();
					UTwoMinWidget_InventorySlot* RinkInventorySlot = FindInventorySlot(RinkItemID);
					RinkInventorySlot->UnRegister();
					FItemInstance* RinkInventoryItem = FindInventoryItem(RinkItemID);
					RinkInventoryItem->UnRegister();
					OnUnEquipment(RinkInventoryItem->ItemID);
				}
				
				InventorySlots[SelectInventoryIndex]->OnRegister(true, EInventorySlotType::Equipment, EquipmentIndex);
				InventoryItem->OnRegister(true, EInventorySlotType::Equipment, EquipmentIndex);
				//EquipmentSlots[EquipmentIndex]->SetInventorySlot(*InventoryItem);
				
				ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
				if (!PlayerCharacter) return FReply::Unhandled();

				PlayerCharacter->GetInventoryComponent()->UpdateInventory();
				
				OnEquipment(InventoryItem->ItemID);
				
				return FReply::Handled();
			}
			else if (SelectEvent ==  ESelectEventType::OpenItemPopup)
			{
				HideInventorySelect();
				
				FItemInstance ItemInstance;
				if (CurInventoryWindowType == EInventoryWindowType::Inventory)
				{
					ItemInstance = InventorySlots[CurInventoryIndex]->GetItemInstance();
				}
				else if (CurInventoryWindowType == EInventoryWindowType::Equipment)
				{
					ItemInstance = EquipmentSlots[CurInventoryIndex]->GetItemInstance();
					
				}
				else if (CurInventoryWindowType == EInventoryWindowType::Quick)
				{
					ItemInstance = QuickSlots[CurInventoryIndex]->GetItemInstance();
				}
				
				ShowItemInfoPopup(ItemInstance.ItemID);
			}
			
			return FReply::Unhandled();
		}
		
		if (CurInventoryWindowType == EInventoryWindowType::Inventory)
		{
			const FItemInstance ItemInstance = InventorySlots[CurInventoryIndex]->GetItemInstance();
			if (ItemInstance.ItemID == 0)
			{
				return FReply::Unhandled();
			}
			
			EItemType ItemType = UTwoMinFunctionLibrary::GetItemType(ItemInstance.ItemID);
			if (ItemType == EItemType::Consume)
			{
				ShowInventorySelect(InventorySlots[CurInventoryIndex], EInventorySelectType::InventoryToConsume);
				return FReply::Handled();	
			}
			else if (ItemType == EItemType::Equipment)
			{
				if (InventorySlots[CurInventoryIndex]->IsRegister())
				{
					ShowInventorySelect(InventorySlots[CurInventoryIndex], EInventorySelectType::InventoryToEquipped);
					return FReply::Handled();
				}

				ShowInventorySelect(InventorySlots[CurInventoryIndex], EInventorySelectType::InventoryToEquipment);
				return FReply::Handled();
			}
			else if (ItemType == EItemType::Etc)
			{
				ShowInventorySelect(InventorySlots[CurInventoryIndex], EInventorySelectType::InventoryToEtc);
				return FReply::Handled();
			}
			
			return FReply::Unhandled();
		}
		else if (CurInventoryWindowType == EInventoryWindowType::Equipment)
		{
			const FItemInstance ItemInstance = EquipmentSlots[CurInventoryIndex]->GetItemInstance();
			if (ItemInstance.ItemID == 0)
			{
				return FReply::Unhandled();
			}
			
			ShowInventorySelect(EquipmentSlots[CurInventoryIndex], EInventorySelectType::EquipToEquipment);
			return FReply::Handled();
		}
		else if (CurInventoryWindowType == EInventoryWindowType::Quick)
		{
			const FItemInstance ItemInstance = QuickSlots[CurInventoryIndex]->GetItemInstance();
			if (ItemInstance.ItemID == 0)
			{
				return FReply::Unhandled();
			}
			
			ShowInventorySelect(QuickSlots[CurInventoryIndex], EInventorySelectType::QuickToConsume);
			return FReply::Handled();
		}
	}
	
	if (InKey == EKeys::Escape || InKey == EKeys::Gamepad_FaceButton_Right)
	{
		if (bIsPopupOpen)
		{
			HideItemInfoPopup();
			return FReply::Handled();
		}
		
		if (bIsSelectOpen == false)
		{
			return FReply::Unhandled();
		}
		
		if (bIsQuickRegister)
		{
			bIsQuickRegister = false;
			
			InventorySelect->ResetAllSelectSlot();
			int32 CurIndex = InventorySelect->GetInventorySelectIndex();
			InventorySelect->MoveToInventorySelectSlot(CurIndex);
			CurInventoryWindowType = EInventoryWindowType::Inventory;
			
			
			return FReply::Handled();
		}
		
		InventorySelect->QuitInventorySelect();
		InventorySelect->SetVisibility(ESlateVisibility::Hidden);
		OnFocusSlot();
		
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Right || InKey == EKeys::D || InKey == EKeys::Gamepad_LeftStick_Right) // + 1
	{
		if (bIsPopupOpen)
		{
			return FReply::Unhandled();
		}
		
		if (bIsSelectOpen)
		{
			if (bIsQuickRegister == false)
			{
				return FReply::Unhandled();
			}
			
			if (CurInventoryWindowType == EInventoryWindowType::Quick)
			{
				int32 NextInventoryIndex = CurInventoryIndex + 1;
				if (NextInventoryIndex > MaxInventoryIndex)
				{
					return FReply::Unhandled();
				}
				
				MoveToQuickSlot(QuickSlots[NextInventoryIndex], NextInventoryIndex);
				return FReply::Handled();
			}
			
			return FReply::Unhandled();
		}
		
		int32 NextInventoryIndex = CurInventoryIndex + 1;
		if (CurInventoryWindowType == EInventoryWindowType::Inventory)
		{
			if (NextInventoryIndex % (MaxColumnIndex + 1) == 0) return FReply::Unhandled();	
			
			MoveToInventorySlot(InventorySlots[NextInventoryIndex], NextInventoryIndex);
			return FReply::Handled();
		}
		else if (CurInventoryWindowType == EInventoryWindowType::Equipment)
		{
			if (NextInventoryIndex > MaxInventoryIndex)
			{
				CurInventoryWindowType = EInventoryWindowType::Inventory;
				NextInventoryIndex = 0;
				MoveToInventorySlot(InventorySlots[NextInventoryIndex], NextInventoryIndex);
				return FReply::Handled();
			}
			
			MoveToEquipmentSlot(EquipmentSlots[NextInventoryIndex], NextInventoryIndex);
			return FReply::Handled();
		}
		else
		{
			if (NextInventoryIndex > MaxInventoryIndex)
			{
				CurInventoryWindowType = EInventoryWindowType::Inventory;
				NextInventoryIndex = 20;
				MoveToInventorySlot(InventorySlots[NextInventoryIndex], NextInventoryIndex);
				return FReply::Handled();
			}
			
			MoveToQuickSlot(QuickSlots[NextInventoryIndex], NextInventoryIndex);
			return FReply::Handled();
		}
	}
	
	if (InKey == EKeys::Left || InKey == EKeys::A || InKey == EKeys::Gamepad_LeftStick_Left) // - 1
	{
		if (bIsPopupOpen)
		{
			return FReply::Unhandled();
		}
		
		if (bIsSelectOpen)
		{
			if (bIsQuickRegister == false)
			{
				return FReply::Unhandled();
			}
			
			if (CurInventoryWindowType == EInventoryWindowType::Quick)
			{
				int32 NextInventoryIndex = CurInventoryIndex - 1;
				if (NextInventoryIndex < 0)
				{
					return FReply::Unhandled();
				}
				
				MoveToQuickSlot(QuickSlots[NextInventoryIndex], NextInventoryIndex);
				return FReply::Handled();
			}
			
			return FReply::Unhandled();
		}
		
		int32 NextInventoryIndex = CurInventoryIndex - 1;
		if (CurInventoryWindowType == EInventoryWindowType::Inventory)
		{
			if (NextInventoryIndex < 0 || NextInventoryIndex % (MaxColumnIndex + 1) == 4)
			{
				if (NextInventoryIndex < 15)
				{
					CurInventoryWindowType = EInventoryWindowType::Equipment;
					// 즉시 가야하는 곳.
					NextInventoryIndex = EquipmentSlots.Max() - 1;
					MoveToEquipmentSlot(EquipmentSlots[NextInventoryIndex], NextInventoryIndex);
					return FReply::Handled();
				}
				else if (NextInventoryIndex >= 15 && NextInventoryIndex < MaxInventoryIndex)
				{
					CurInventoryWindowType = EInventoryWindowType::Quick;
					NextInventoryIndex = QuickSlots.Max() - 1;
					MoveToQuickSlot(QuickSlots[NextInventoryIndex], NextInventoryIndex);
					return FReply::Handled();
				}
				
				return FReply::Unhandled();
			}		
			
			MoveToInventorySlot(InventorySlots[NextInventoryIndex], NextInventoryIndex);
			return FReply::Handled();
		}
		else if (CurInventoryWindowType == EInventoryWindowType::Equipment)
		{
			if (NextInventoryIndex < 0) return FReply::Unhandled();
			
			MoveToEquipmentSlot(EquipmentSlots[NextInventoryIndex], NextInventoryIndex);
			return FReply::Handled();
		}
		else
		{
			if (NextInventoryIndex < 0) return FReply::Unhandled();
			
			MoveToQuickSlot(QuickSlots[NextInventoryIndex], NextInventoryIndex);
			return FReply::Handled();
		}
	}
	
	if (InKey == EKeys::Up || InKey == EKeys::W || InKey == EKeys::Gamepad_LeftStick_Up) // - 5
	{
		if (bIsPopupOpen)
		{
			return FReply::Unhandled();
		}
		
		if (bIsSelectOpen)
		{
			if (bIsQuickRegister) return FReply::Unhandled(); 
				
			InventorySelect->SetInventorySelectIndex(-1);
			int32 NextIndex = InventorySelect->GetInventorySelectIndex();
			InventorySelect->MoveToInventorySelectSlot(NextIndex);
			
			return FReply::Handled();
		}
		
		int32 NextInventoryIndex = CurInventoryIndex - 5;
		if (CurInventoryWindowType == EInventoryWindowType::Inventory)
		{
			if (NextInventoryIndex < 0) return FReply::Unhandled();
		
			MoveToInventorySlot(InventorySlots[NextInventoryIndex], NextInventoryIndex);
			return FReply::Handled();	
		}
		else if (CurInventoryWindowType == EInventoryWindowType::Quick)
		{
			CurInventoryWindowType = EInventoryWindowType::Equipment;
			NextInventoryIndex = 0;
			MoveToEquipmentSlot(EquipmentSlots[NextInventoryIndex], NextInventoryIndex);
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Down || InKey == EKeys::S || InKey == EKeys::Gamepad_LeftStick_Down) // + 5
	{
		if (bIsPopupOpen)
		{
			return FReply::Unhandled();
		}
		
		if (bIsSelectOpen)
		{
			if (bIsQuickRegister) return FReply::Unhandled();
			
			InventorySelect->SetInventorySelectIndex(1);
			int32 NextIndex = InventorySelect->GetInventorySelectIndex();
			InventorySelect->MoveToInventorySelectSlot(NextIndex);
			
			return FReply::Handled();
		}
		
		int32 NextInventoryIndex = CurInventoryIndex + 5;
		if (CurInventoryWindowType == EInventoryWindowType::Inventory)
		{
			if (NextInventoryIndex > MaxInventoryIndex) return FReply::Unhandled();
		
			MoveToInventorySlot(InventorySlots[NextInventoryIndex], NextInventoryIndex);
			return FReply::Handled();	
		}
		else if (CurInventoryWindowType == EInventoryWindowType::Equipment)
		{
			CurInventoryWindowType = EInventoryWindowType::Quick;
			NextInventoryIndex = 0;
			MoveToQuickSlot(QuickSlots[NextInventoryIndex], NextInventoryIndex);
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

UTwoMinWidget_InventorySlot* UTwoMinWidget_InventoryUI::FindInventorySlot(int32 ItemID)
{
	TArray<UTwoMinWidget_InventorySlot*> InventorySlots = InventoryWindow->GetInventorySlots();
	if (InventorySlots.IsEmpty()) return nullptr;
	
	for (int32 Index = 0; Index < InventorySlots.Num(); Index++)
	{
		if (InventorySlots[Index]->GetItemInstance().ItemID == ItemID)
		{
			return InventorySlots[Index];
		}
	}
	
	return nullptr;
}

UTwoMinWidget_InventorySlot* UTwoMinWidget_InventoryUI::FindQuickSlot(int32 ItemID)
{
	TArray<UTwoMinWidget_InventorySlot*> QuickSlots = QuickWindow->GetInventorySlots();
	if (QuickSlots.IsEmpty()) return nullptr;
	
	for (int32 Index = 0; Index < QuickSlots.Num(); Index++)
	{
		if (QuickSlots[Index]->GetItemInstance().ItemID == ItemID)
		{
			return QuickSlots[Index];
		}
	}
	
	return nullptr;
}

void UTwoMinWidget_InventoryUI::MoveToInventorySlot(UTwoMinWidget_InventorySlot* InvenSlot, int32 NextIndex)
{
	InvenSlot->SetFocus();
	InventoryWindow->SetCurInventoryIndex(NextIndex);
}

void UTwoMinWidget_InventoryUI::MoveToEquipmentSlot(UTwoMinWidget_InventorySlot* EquipSlot, int32 NextIndex)
{
	EquipSlot->SetFocus();
	EquipmentWindow->SetCurInventoryIndex(NextIndex);
}

void UTwoMinWidget_InventoryUI::MoveToQuickSlot(UTwoMinWidget_InventorySlot* QuickSlot, int32 NextIndex)
{
	QuickSlot->SetFocus();
	QuickWindow->SetCurInventoryIndex(NextIndex);
}

void UTwoMinWidget_InventoryUI::ShowInventorySelect(const UTwoMinWidget_InventorySlot* CurSlot, 
	const EInventorySelectType NewInventorySelectType)
{
	InventorySelect->SetVisibility(ESlateVisibility::Visible);
	InventorySelect->SetInventorySelectType(NewInventorySelectType);
	
	FGeometry CanvasGeo = GetRootWidget()->GetCachedGeometry();
	FGeometry SlotGeo   = CurSlot->GetCachedGeometry();

	FVector2D Abs = SlotGeo.LocalToAbsolute(FVector2D(SlotGeo.GetLocalSize().X, 0));
	FVector2D CanvasLocal = CanvasGeo.AbsoluteToLocal(Abs);

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(InventorySelect->Slot);
	CanvasSlot->SetPosition(CanvasLocal);
}

void UTwoMinWidget_InventoryUI::HideInventorySelect()
{
	InventorySelect->QuitInventorySelect();
	InventorySelect->SetVisibility(ESlateVisibility::Hidden);
	OnFocusSlot();
}

void UTwoMinWidget_InventoryUI::ShowItemInfoPopup(const int32 CurItemID)
{
	ItemInfoPopup->SetVisibility(ESlateVisibility::Visible);
	ItemInfoPopup->SetItemInformation(CurItemID);
}

void UTwoMinWidget_InventoryUI::HideItemInfoPopup()
{
	ItemInfoPopup->HidePopup();
	ItemInfoPopup->SetVisibility(ESlateVisibility::Hidden);
	OnFocusSlot();
}

FItemInstance* UTwoMinWidget_InventoryUI::FindInventoryItem(int32 ItemID)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return nullptr;
	
	FItemInstance* InventoryItem = PlayerCharacter->GetInventoryComponent()->FindItemInstance(ItemID);
	return InventoryItem;
}

void UTwoMinWidget_InventoryUI::OnFocusSlot()
{
	TArray<UTwoMinWidget_InventorySlot*> EquipmentSlots = EquipmentWindow->GetInventorySlots();
	TArray<UTwoMinWidget_InventorySlot*> QuickSlots = QuickWindow->GetInventorySlots();
	TArray<UTwoMinWidget_InventorySlot*> InventorySlots = InventoryWindow->GetInventorySlots();
	
	if (InventorySlots.IsEmpty() || EquipmentSlots.IsEmpty() || QuickSlots.IsEmpty()) return;

	if (CurInventoryWindowType == EInventoryWindowType::Inventory)
	{
		const int32 CurInventoryIndex = InventoryWindow->GetCurInventoryIndex();
		MoveToInventorySlot(InventorySlots[CurInventoryIndex], CurInventoryIndex);
	}
	else if (CurInventoryWindowType == EInventoryWindowType::Equipment)
	{
		const int32 CurInventoryIndex = EquipmentWindow->GetCurInventoryIndex();
		MoveToEquipmentSlot(EquipmentSlots[CurInventoryIndex], CurInventoryIndex);
	}
	else
	{
		const int32 CurInventoryIndex = QuickWindow->GetCurInventoryIndex();
		MoveToQuickSlot(QuickSlots[CurInventoryIndex], CurInventoryIndex);
	}
}


void UTwoMinWidget_InventoryUI::OnEquipment(int32 ItemID)
{
	const ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;
	
	UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;
	
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	FItemEquipmentData Item = GI->ItemDataManager->GetItemEquipmentData(ItemID);

	FGameplayEffectSpecHandle Spec = 
		ASC->MakeOutgoingSpec(
			PlayerCharacter->GetEquipStatusEffect(),
			1.f,
			ASC->MakeEffectContext()
		);
	
	if (!Spec.IsValid()) return;
	
	if (Item.EquipmentPower.IsEmpty()) return;

	FGameplayTag WeaponTag;
	if (Item.EquipmentType == EEquipmentType::Weapon_Right)
	{
		WeaponTag = TwoMinGameplayTag::Shared_Ability_Weapon_Right;
	}
	else if (Item.EquipmentType == EEquipmentType::Weapon_Left)
	{
		WeaponTag = TwoMinGameplayTag::Shared_Ability_Weapon_Left;
	}
	else
	{
		// todo : 두손검 추가
		//
	}
	
	UTwoMinGameplayAbility* Ability = ASC->GetActiveAbility(WeaponTag);
	if (!Ability) return;
	
	UTwoMinGameplayAbility_WeaponSpawn* WeaponSpawn =  Cast<UTwoMinGameplayAbility_WeaponSpawn>(Ability);
	if (!WeaponSpawn) return;
	
	WeaponSpawn->OnChangeWeapon(Item.WeaponClass, false);
	
	int32 AttackPower = 0;
	int32 DefensePower = 0;
	int32 MaxHealth = 0;
	int32 MaxStamina = 0;
	for (auto EquipmentPower : Item.EquipmentPower)
	{
		if (EquipmentPower.Key == EStatusType::Attack)
		{
			AttackPower = EquipmentPower.Value;
		}
		else if (EquipmentPower.Key == EStatusType::Defense)
		{
			DefensePower = EquipmentPower.Value;
		}
		else if (EquipmentPower.Key == EStatusType::MaxHealth)
		{
			MaxHealth = EquipmentPower.Value;
		}
		else if (EquipmentPower.Key == EStatusType::MaxStamina)
		{
			MaxStamina = EquipmentPower.Value;
		}
	}
	
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Equipment_AttackPower, AttackPower);
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Equipment_DefensePower, DefensePower);
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Equipment_MaxHealth, MaxHealth);
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Equipment_MaxStamina, MaxStamina);
	
	FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	ASC->AddEquippedItemEffect(Item.ItemDataBase.ItemID, Handle);
	EquipmentWindow->UpdateStatusText();
}

void UTwoMinWidget_InventoryUI::OnUnEquipment(int32 ItemID)
{
	const ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;
	
	UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;
	
	FActiveGameplayEffectHandle* Handle = ASC->FindEquippedItemEffect(ItemID);
	if (!Handle) return;
		
	ASC->RemoveActiveGameplayEffect(*Handle);
	ASC->RemoveEquippedItemEffect(ItemID);
	EquipmentWindow->UpdateStatusText();
}
