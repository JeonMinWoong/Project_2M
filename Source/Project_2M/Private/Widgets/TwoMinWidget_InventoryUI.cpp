// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_InventoryUI.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/InventoryComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/ItemDataManager.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidget_EquipmentSlot.h"
#include "Widgets/TwoMinWidget_InventorySlot.h"
#include "Widgets/TwoMinWidget_InventoryWindow.h"

UTwoMinWidget_InventoryUI::UTwoMinWidget_InventoryUI()
{
	CurInventoryWindowType = EInventoryWindowType::Inventory;
}

void UTwoMinWidget_InventoryUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	InventorySelect->SetVisibility(ESlateVisibility::Hidden);
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
	const FKey InKey = InKeyEvent.GetKey();
	
	if (InKey == EKeys::Enter || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
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
				
				InventorySlots[SelectInventoryIndex]->OnRegister(true, EInventorySlotType::Quick, CurQuickIndex);
				InventoryItem->OnRegister(true, EInventorySlotType::Quick, CurQuickIndex);
				QuickSlots[CurQuickIndex]->SetInventorySlot(*InventoryItem);
				
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
					
					PlayerCharacter->OpenInventoryProcess();
					PlayerCharacter->GetInventoryComponent()->UseItem(ItemInstance.ItemID);
					PlayerCharacter->GetInventoryComponent()->UpdateInventory();
					
					UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
						PlayerCharacter, 
						TwoMinGameplayTag::Player_Event_UseItem, 
						FGameplayEventData()
					);
				}
				
				return FReply::Handled();
			}
			else if (SelectEvent == ESelectEventType::QuickToConsume_NonRegister)
			{
				HideInventorySelect();
				
				int32 SelectQuickIndex = QuickWindow->GetCurInventoryIndex();
				FItemInstance ItemInstance = QuickSlots[SelectQuickIndex]->GetItemInstance();
				int32 RinkItemID = QuickSlots[SelectQuickIndex]->UnRegister();
				UTwoMinWidget_InventorySlot* RinkInventorySlot = FindInventorySlot(RinkItemID);
				RinkInventorySlot->UnRegister();
				FItemInstance* InventoryItem = FindInventoryItem(ItemInstance.ItemID);
				InventoryItem->UnRegister();
				
				// todo : 실제 퀵슬롯에서 소비 아이템 등록 해제.
				
				return FReply::Handled();
			}
			else if (SelectEvent == ESelectEventType::QuickToConsume_Use)
			{
				HideInventorySelect();
				
				if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn()))
				{
					int32 SelectQuickIndex = QuickWindow->GetCurInventoryIndex();
					FItemInstance ItemInstance = QuickSlots[SelectQuickIndex]->GetItemInstance();
					
					PlayerCharacter->OpenInventoryProcess();
					PlayerCharacter->GetInventoryComponent()->UseItem(ItemInstance.ItemID);
					PlayerCharacter->GetInventoryComponent()->UpdateInventory();
					
					UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
						PlayerCharacter, 
						TwoMinGameplayTag::Player_Event_UseItem, 
						FGameplayEventData()
					);
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
				}
				
				InventorySlots[SelectInventoryIndex]->OnRegister(true, EInventorySlotType::Equipment, EquipmentIndex);
				InventoryItem->OnRegister(true, EInventorySlotType::Equipment, EquipmentIndex);
				EquipmentSlots[EquipmentIndex]->SetInventorySlot(*InventoryItem);
				
				// todo : 장비 능력치 적용.
				
				return FReply::Handled();
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
					return FReply::Unhandled();
				}

				ShowInventorySelect(InventorySlots[CurInventoryIndex], EInventorySelectType::InventoryToEquipment);
				
				return FReply::Handled();
			}
			
			return FReply::Unhandled();
		}
		else if (CurInventoryWindowType == EInventoryWindowType::Equipment)
		{
			// todo : 장비 슬롯
		}
		else if (CurInventoryWindowType == EInventoryWindowType::Quick)
		{
			const FItemInstance ItemInstance = QuickSlots[CurInventoryIndex]->GetItemInstance();
			if (ItemInstance.ItemID == 0)
			{
				return FReply::Unhandled();
			}
			
			ShowInventorySelect(QuickSlots[CurInventoryIndex], EInventorySelectType::QuickToConsume);
		}
	}
	
	if (InKey == EKeys::P || InKey == EKeys::Gamepad_FaceButton_Right)
	{
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