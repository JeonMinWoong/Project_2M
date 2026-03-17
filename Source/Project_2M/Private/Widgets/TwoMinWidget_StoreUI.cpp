// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_StoreUI.h"

#include "TwoMinDebugHelper.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/InventoryComponent.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/ItemDataManager.h"
#include "System/InteractionActor_NPC.h"
#include "Widgets/TwoMinWidget_GoldInfo.h"
#include "Widgets/TwoMinWidget_InventorySlot.h"
#include "Widgets/TwoMinWidget_InventoryWindow.h"
#include "Widgets/TwoMinWidget_ItemInfoPopup.h"
#include "Widgets/TwoMinWidget_StoreDealPopup.h"
#include "Widgets/TwoMinWidget_StoreDealText.h"
#include "Widgets/TwoMinWidget_StoreSelect.h"
#include "Widgets/TwoMinWidget_StoreSlot.h"
#include "Widgets/TwoMinWidget_StoreWindow.h"

class UTwoMinWidget_InventorySlot;

void UTwoMinWidget_StoreUI::InitStoreUI(const AInteractionActor_NPC* NPC, const ATwoMinPlayerCharacter* PlayerCharacter)
{
	CurStoreWindowType = EInventoryWindowType::Store;
	
	InitStoreSellList(NPC);
	InitStoreGold(PlayerCharacter);
	HideStoreSelect();
	HideStoreInfoPopup();
	StoreDealText->SetVisibility(ESlateVisibility::Hidden);
}

void UTwoMinWidget_StoreUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (StoreDealInput != EStoreDealInputType::None)
	{
		float CurHoldDealInputTime = GetWorld()->GetRealTimeSeconds() - StartHoldDealInputTime;
		if (CurHoldDealInputTime > HoldDealInputDelay)
		{
			CurHoldIncreaseInputTime += InDeltaTime;
			if (CurHoldIncreaseInputTime >= HoldIncreaseInputTime)
			{
				CurHoldIncreaseInputTime = 0;
				IncreaseInputCount++;
			}
		}
	}
}

void UTwoMinWidget_StoreUI::InitStoreSellList(const AInteractionActor_NPC* NPC) const
{
	const TSet<int32> StoreSellList = NPC->GetStoreList();
	if (StoreSellList.IsEmpty() || StoreSellList.Num() == 0) return;
	
	StoreWindow->ResettingStoreList(StoreSellList);
}

void UTwoMinWidget_StoreUI::InitStoreGold(const ATwoMinPlayerCharacter* PlayerCharacter) const
{
	if (!PlayerCharacter) return;
	
	const UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;
	
	const int32 CurGold = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentGoldAttribute());
	StoreGoldInfo->SetGoldText(CurGold);
}

FReply UTwoMinWidget_StoreUI::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InKey = InKeyEvent.GetKey();
	
	TArray<TObjectPtr<UTwoMinWidget_StoreSlot>> StoreSlots = StoreWindow->GetStoreSlots();
	TArray<TObjectPtr<UTwoMinWidget_InventorySlot>> InventorySlots = InventoryWindow->GetInventorySlots();
	
	if (StoreSlots.IsEmpty() || InventorySlots.IsEmpty()) return FReply::Unhandled();
	
	int32 CurStoreIndex = StoreWindow->GetCurStoreIndex();
	int32 CurInventoryIndex = InventoryWindow->GetCurInventoryIndex();
	int32 MaxInventoryIndex = InventoryWindow->GetMaxInventoryIndex();
	int32 InventoryMaxColumnIndex = InventoryWindow->GetMaxColumnIndex();	
	
	bool bIsSelectOpen = StoreSelect->IsOpen();
	bool bIsPopupOpen = ItemInfoPopup->IsPopupOpen();
	bool bIsDealPopupOpen = StoreDealPopup->IsOpenPopup();
	
	if (InKey == EKeys::Enter || InKey == EKeys::F || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
		if (bIsDealPopupOpen)
		{
			const int32 ButtonIndex = StoreDealPopup->GetCurButtonIndex();
			if (ButtonIndex == 1)
			{
				HideStoreInfoPopup();
				return FReply::Handled();
			}
			
			int32 CurItemID = 0;
			if (StoreDealPopup->GetStoreDealPopupType() == EStoreDealPopupType::Sell)
			{
				ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
				if (!PlayerCharacter) return FReply::Unhandled();
				
				CurItemID = InventorySlots[CurInventoryIndex]->GetItemInstance().ItemID;
				if (CurItemID == 0) return FReply::Unhandled(); 
				
				const int32 CurDealValue = StoreDealPopup->GetCurDealValue();
				if (CurDealValue == 0) return FReply::Unhandled();
				
				PlayerCharacter->GetInventoryComponent()->SellInventoryItem(CurItemID, CurDealValue);
				InitStoreGold(PlayerCharacter);
				
				AActor* InteractionActor = PlayerCharacter->GetInteractionActor();
				if (!InteractionActor) return FReply::Unhandled();
			
				AInteractionActor_NPC* NPC = Cast<AInteractionActor_NPC>(InteractionActor);
				if (!NPC) return FReply::Unhandled();
					
				StoreDealText->SetTextValue(NPC->GetNPCDealTextStr(EStoreDealTextType::Sell_Success));
				
				HideStoreInfoPopup();
				return FReply::Handled();
			}
			
			ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
			if (!PlayerCharacter) return FReply::Unhandled();
				
			CurItemID = StoreSlots[CurStoreIndex]->GetItemID();
			if (CurItemID == 0) return FReply::Unhandled(); 
				
			const int32 CurDealValue = StoreDealPopup->GetCurDealValue();
			if (CurDealValue == 0) return FReply::Unhandled();
			
			PlayerCharacter->GetInventoryComponent()->BuyInventoryItem(CurItemID, CurDealValue);
			InitStoreGold(PlayerCharacter);
			
			AActor* InteractionActor = PlayerCharacter->GetInteractionActor();
			if (!InteractionActor) return FReply::Unhandled();
			
			AInteractionActor_NPC* NPC = Cast<AInteractionActor_NPC>(InteractionActor);
			if (!NPC) return FReply::Unhandled();
					
			StoreDealText->SetTextValue(NPC->GetNPCDealTextStr(EStoreDealTextType::Buy_Success));

			HideStoreInfoPopup();
			return FReply::Handled();
		}
		
		if (bIsPopupOpen)
		{
			HideItemInfoPopup();
			return FReply::Handled();
		}
		
		if (bIsSelectOpen)
		{
			int32 CurIndex = StoreSelect->GetStoreSelectIndex();
			ESelectEventType SelectEvent = StoreSelect->OnTriggerEvent(CurIndex);
			if (SelectEvent == ESelectEventType::Cancel)
			{
				HideStoreSelect();
				return FReply::Handled();
			}
			else if (SelectEvent == ESelectEventType::OpenItemPopup)
			{
				HideStoreSelect();
				
				int32 CurItemID = 0;
				if (CurStoreWindowType == EInventoryWindowType::Inventory)
				{
					CurItemID = InventorySlots[CurInventoryIndex]->GetItemInstance().ItemID;
				}
				else if (CurStoreWindowType == EInventoryWindowType::Store)
				{
					CurItemID = StoreSlots[CurStoreIndex]->GetItemID();
				}
				
				ShowItemInfoPopup(CurItemID);
				return FReply::Handled();
			}
			else if (SelectEvent == ESelectEventType::InventoryToStore_Sell)
			{
				HideStoreSelect();
				
				int32 CurItemID = InventorySlots[CurInventoryIndex]->GetItemInstance().ItemID;
				bool IsPossibleDeal = ShowStoreDealPopup(CurItemID, EStoreDealPopupType::Sell);
				if (IsPossibleDeal == false)
				{
					// todo : 설명
					return FReply::Unhandled();
				}
				
				return FReply::Handled();
			}
			else if (SelectEvent == ESelectEventType::StoreToInventory_Buy)
			{
				HideStoreSelect();
				
				int32 CurItem = StoreSlots[CurStoreIndex]->GetItemID();
				bool IsPossibleDeal = ShowStoreDealPopup(CurItem, EStoreDealPopupType::Buy);
				if (IsPossibleDeal == false)
				{
					EStoreDealTextType CurStoreDealTextType = StoreDealPopup->GetStoreDealTextType();
					ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
					if (!PlayerCharacter) return FReply::Unhandled();
					
					AActor* InteractionActor = PlayerCharacter->GetInteractionActor();
					if (!InteractionActor) return FReply::Unhandled();
			
					AInteractionActor_NPC* NPC = Cast<AInteractionActor_NPC>(InteractionActor);
					if (!NPC) return FReply::Unhandled();
					
					StoreDealText->SetTextValue(NPC->GetNPCDealTextStr(CurStoreDealTextType));
					HideStoreInfoPopup();
					
					return FReply::Unhandled();
				}
				
				return FReply::Handled();
			}
			
			return FReply::Unhandled();
		}
		
		if (CurStoreWindowType == EInventoryWindowType::Store)
		{
			const int32 ItemID = StoreSlots[CurStoreIndex]->GetItemID();
			if (ItemID == 0) return FReply::Unhandled();

			ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
			if (!PlayerCharacter) return FReply::Unhandled();
			
			EItemType ItemType = UTwoMinFunctionLibrary::GetItemType(ItemID);
			if (ItemType == EItemType::Equipment)
			{
				if (FItemInstance* FindItemInstance = PlayerCharacter->GetInventoryComponent()->FindItemInstance(ItemID))
				{
					if (FindItemInstance->bIsRegister)
					{
						ShowStoreSelect(StoreSlots[CurStoreIndex], EStoreSelectType::NoDeal);
						return FReply::Handled();
					}
				}
				
				ShowStoreSelect(StoreSlots[CurStoreIndex], EStoreSelectType::StoreToInventory);
				return FReply::Handled();
			}
			
			ShowStoreSelect(StoreSlots[CurStoreIndex], EStoreSelectType::StoreToInventory);
			return FReply::Handled();
		}
		else if (CurStoreWindowType == EInventoryWindowType::Inventory)
		{
			const FItemInstance ItemInstance = InventorySlots[CurInventoryIndex]->GetItemInstance();
			if (ItemInstance.ItemID == 0) return FReply::Unhandled();

			EItemType ItemType = UTwoMinFunctionLibrary::GetItemType(ItemInstance.ItemID);
			if (ItemType == EItemType::Equipment || ItemType == EItemType::Consume)
			{
				// 장착 중 확인 해야함.
				if (InventorySlots[CurInventoryIndex]->IsRegister())
				{
					ShowStoreSelect(InventorySlots[CurInventoryIndex], EStoreSelectType::NoDeal);
					return FReply::Handled();
				}

				ShowStoreSelect(InventorySlots[CurInventoryIndex], EStoreSelectType::InventoryToStore);
				return FReply::Handled();
			}

			ShowStoreSelect(InventorySlots[CurInventoryIndex], EStoreSelectType::InventoryToStore);
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::P || InKey == EKeys::Gamepad_FaceButton_Right)
	{
		if (bIsDealPopupOpen)
		{
			HideStoreInfoPopup();
			return FReply::Handled();	
		}
		
		if (bIsPopupOpen)
		{
			HideItemInfoPopup();
			return FReply::Handled();
		}
		
		if (bIsSelectOpen)
		{
			StoreSelect->QuitInventorySelect();
			UpdateFocusSlot();
			return FReply::Handled();
		}
		
		if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn()))
		{
			UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetPlayerUIComponent();
			if (PlayerUIComponent->IsStoreWidgetOpen() == false) return FReply::Unhandled();
			
			AActor* InteractionActor = PlayerCharacter->GetInteractionActor();
			if (!InteractionActor) return FReply::Unhandled();
			
			AInteractionActor_NPC* NPC = Cast<AInteractionActor_NPC>(InteractionActor);
			if (!NPC) return FReply::Unhandled();
			
			StoreSlots[0]->SetFocus();
			PlayerUIComponent->OpenStoreWidget(PlayerCharacter, NPC, false);	
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Q || InKey == EKeys::Gamepad_LeftShoulder)
	{
		if (bIsDealPopupOpen)
		{
			if (StoreDealInput != EStoreDealInputType::Decrease)
			{
				ResetHoldDealInput();
				StoreDealInput = EStoreDealInputType::Decrease;
				StartHoldDealInputTime = GetWorld()->GetRealTimeSeconds();	
			}
			
			int32 DecreaseValue = -IncreaseInputCount;
			int32 CurItemID = 0;
			switch (StoreDealPopup->GetStoreDealPopupType()) {
			case EStoreDealPopupType::Sell:
				CurItemID = InventorySlots[CurInventoryIndex]->GetItemInstance().ItemID;
				break;
			case EStoreDealPopupType::Buy:
				CurItemID = StoreSlots[CurStoreIndex]->GetItemID();
				break;
			}
			
			if (CurItemID == 0) return FReply::Unhandled();
			
			UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
			if (!GI) return FReply::Unhandled();
				
			FItemData CurItemData = GI->ItemDataManager->GetItemDataBase(CurItemID);
			StoreDealPopup->SetModifyDealProcess(CurItemData, DecreaseValue);
			
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::E || InKey == EKeys::Gamepad_RightShoulder)
	{
		if (bIsDealPopupOpen)
		{
			if (StoreDealInput != EStoreDealInputType::Increase)
			{
				ResetHoldDealInput();
				StoreDealInput = EStoreDealInputType::Increase;
				StartHoldDealInputTime = GetWorld()->GetRealTimeSeconds();	
			}
			
			int32 IncreaseValue = IncreaseInputCount;
			int32 CurItemID = 0;
			switch (StoreDealPopup->GetStoreDealPopupType()) {
			case EStoreDealPopupType::Sell:
				CurItemID = InventorySlots[CurInventoryIndex]->GetItemInstance().ItemID;
				break;
			case EStoreDealPopupType::Buy:
				CurItemID = StoreSlots[CurStoreIndex]->GetItemID();
				break;
			}
			
			if (CurItemID == 0) return FReply::Unhandled();
			
			UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
			if (!GI) return FReply::Unhandled();
				
			FItemData CurItemData = GI->ItemDataManager->GetItemDataBase(CurItemID);
			StoreDealPopup->SetModifyDealProcess(CurItemData, IncreaseValue);
			
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Right || InKey == EKeys::D || InKey == EKeys::Gamepad_LeftStick_Right)
	{
		if (bIsDealPopupOpen)
		{
			StoreDealPopup->SetSelectButton(1);
			return FReply::Handled();	
		}
		
		if (bIsPopupOpen)
		{
			return FReply::Unhandled();
		}
		
		if (bIsSelectOpen)
		{
			return FReply::Unhandled();
		}
		
		if (CurStoreWindowType == EInventoryWindowType::Inventory)
		{
			int32 NextInventoryIndex = CurInventoryIndex + 1;
			if (NextInventoryIndex % (InventoryMaxColumnIndex + 1) == 0) return FReply::Unhandled();	
			
			InventorySlots[NextInventoryIndex]->SetFocus();
			InventoryWindow->SetCurInventoryIndex(NextInventoryIndex);
			return FReply::Handled();
		}
		else if (CurStoreWindowType == EInventoryWindowType::Store)
		{
			CurStoreWindowType = EInventoryWindowType::Inventory;
			StoreWindow->SetCurStoreIndex(0);
			
			CurInventoryIndex = 0;
			InventorySlots[CurInventoryIndex]->SetFocus();
			InventoryWindow->SetCurInventoryIndex(CurInventoryIndex);
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Left || InKey == EKeys::A || InKey == EKeys::Gamepad_LeftStick_Left)
	{
		if (bIsDealPopupOpen)
		{
			StoreDealPopup->SetSelectButton(0);
			return FReply::Handled();	
		}
		
		if (bIsPopupOpen)
		{
			return FReply::Unhandled();
		}
		
		if (bIsSelectOpen)
		{
			return FReply::Unhandled();
		}
		
		if (CurStoreWindowType == EInventoryWindowType::Inventory)
		{
			int32 NextInventoryIndex = CurInventoryIndex - 1;
			if (NextInventoryIndex < 0 || NextInventoryIndex % (InventoryMaxColumnIndex + 1) == 4)
			{
				CurStoreWindowType = EInventoryWindowType::Store;

				CurStoreIndex = 0;
				StoreWindow->SetFocusSlot(CurStoreIndex);
				return FReply::Handled();
			}		
			
			InventorySlots[NextInventoryIndex]->SetFocus();
			InventoryWindow->SetCurInventoryIndex(NextInventoryIndex);
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Up || InKey == EKeys::W || InKey == EKeys::Gamepad_LeftStick_Up)
	{
		if (bIsDealPopupOpen)
		{
			return FReply::Handled();	
		}
		
		if (bIsPopupOpen)
		{
			return FReply::Unhandled();
		}
		
		if (bIsSelectOpen)
		{
			StoreSelect->SetStoreSelectIndex(-1);
			const int32 NextIndex = StoreSelect->GetStoreSelectIndex();
			StoreSelect->OnFocusStoreSelectSlot(NextIndex);
			return FReply::Handled();
		}
		
		if (CurStoreWindowType == EInventoryWindowType::Inventory)
		{
			int32 NextInventoryIndex = CurInventoryIndex - 5;
			if (NextInventoryIndex < 0) return FReply::Unhandled();
		
			InventorySlots[NextInventoryIndex]->SetFocus();
			InventoryWindow->SetCurInventoryIndex(NextInventoryIndex);
			return FReply::Handled();	
		}
		else if (CurStoreWindowType == EInventoryWindowType::Store)
		{
			int32 NextStoreIndex = CurStoreIndex - 1;
			if (NextStoreIndex < 0) return FReply::Unhandled();
			if (StoreWindow->IsEmptySlotIndex(NextStoreIndex)) return FReply::Unhandled();
		
			StoreWindow->SetFocusSlot(NextStoreIndex);
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Down || InKey == EKeys::S || InKey == EKeys::Gamepad_LeftStick_Down)
	{
		if (bIsDealPopupOpen)
		{
			return FReply::Handled();	
		}
		
		if (bIsPopupOpen)
		{
			return FReply::Unhandled();
		}
		
		if (bIsSelectOpen)
		{
			StoreSelect->SetStoreSelectIndex(1);
			const int32 NextIndex = StoreSelect->GetStoreSelectIndex();
			StoreSelect->OnFocusStoreSelectSlot(NextIndex);
			return FReply::Handled();
		}
		
		if (CurStoreWindowType == EInventoryWindowType::Inventory)
		{
			int32 NextInventoryIndex = CurInventoryIndex + 5;
			if (NextInventoryIndex > MaxInventoryIndex) return FReply::Unhandled();
		
			InventorySlots[NextInventoryIndex]->SetFocus();
			InventoryWindow->SetCurInventoryIndex(NextInventoryIndex);
			return FReply::Handled();	
		}
		else if (CurStoreWindowType == EInventoryWindowType::Store)
		{
			int32 NextStoreIndex = CurStoreIndex + 1;
			if (NextStoreIndex > StoreWindow->GetMaxStoreIndex()) return FReply::Unhandled();
			if (StoreWindow->IsEmptySlotIndex(NextStoreIndex)) return FReply::Unhandled();
		
			StoreWindow->SetFocusSlot(NextStoreIndex);
			return FReply::Handled();	
		}
		
		return FReply::Unhandled();
	}
	
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

FReply UTwoMinWidget_StoreUI::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InKey = InKeyEvent.GetKey();
	bool bIsDealPopupOpen = StoreDealPopup->IsOpenPopup();
	
	if (InKey == EKeys::Q || InKey == EKeys::E || InKey == EKeys::Gamepad_LeftShoulder || InKey == EKeys::Gamepad_RightShoulder)
	{
		if (bIsDealPopupOpen)
		{
			if (StoreDealInput != EStoreDealInputType::None)
			{
				ResetHoldDealInput();
			}
			
			return FReply::Handled();
		}
	}
	
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UTwoMinWidget_StoreUI::ResetHoldDealInput()
{
	IncreaseInputCount = 1;
	StartHoldDealInputTime = 0;
	CurHoldIncreaseInputTime = 0;
	StoreDealInput = EStoreDealInputType::None;
}

void UTwoMinWidget_StoreUI::ShowStoreSelect(const UTwoMinWidgetBase* CurSlot, const EStoreSelectType NewStoreSelectType)
{
	StoreSelect->SetVisibility(ESlateVisibility::Visible);
	StoreSelect->SetStoreSelectType(NewStoreSelectType);
	
	FGeometry CanvasGeo = GetRootWidget()->GetCachedGeometry();
	FGeometry SlotGeo   = CurSlot->GetCachedGeometry();

	FVector2D Abs = SlotGeo.LocalToAbsolute(FVector2D(SlotGeo.GetLocalSize().X, 0));
	FVector2D CanvasLocal = CanvasGeo.AbsoluteToLocal(Abs);

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(StoreSelect->Slot);
	CanvasSlot->SetPosition(CanvasLocal);
}

void UTwoMinWidget_StoreUI::HideStoreSelect()
{
	StoreSelect->QuitInventorySelect();
	StoreSelect->SetVisibility(ESlateVisibility::Hidden);
	UpdateFocusSlot();
}

void UTwoMinWidget_StoreUI::UpdateFocusSlot()
{
	TArray<UTwoMinWidget_StoreSlot*> StoreSlots = StoreWindow->GetStoreSlots();
	TArray<UTwoMinWidget_InventorySlot*> InventorySlots = InventoryWindow->GetInventorySlots();
	
	if (InventorySlots.IsEmpty() || StoreSlots.IsEmpty()) return;

	if (CurStoreWindowType == EInventoryWindowType::Inventory)
	{
		const int32 CurInventoryIndex = InventoryWindow->GetCurInventoryIndex();
		InventoryWindow->GetInventorySlots()[CurInventoryIndex]->SetFocus();
		InventoryWindow->SetCurInventoryIndex(CurInventoryIndex);
	}
	else if (CurStoreWindowType == EInventoryWindowType::Store)
	{
		const int32 CurStoreIndex = StoreWindow->GetCurStoreIndex();
		StoreWindow->SetFocusSlot(CurStoreIndex);
	}
}

void UTwoMinWidget_StoreUI::ShowItemInfoPopup(const int32 CurItemID)
{
	ItemInfoPopup->SetVisibility(ESlateVisibility::Visible);
	ItemInfoPopup->SetItemInformation(CurItemID);
}

void UTwoMinWidget_StoreUI::HideItemInfoPopup()
{
	ItemInfoPopup->HidePopup();
	ItemInfoPopup->SetVisibility(ESlateVisibility::Hidden);
	UpdateFocusSlot();
}

bool UTwoMinWidget_StoreUI::ShowStoreDealPopup(const int32 CurItemID, EStoreDealPopupType NewStoreDealPopupType)
{
	return StoreDealPopup->TryStoreDealItemInformation(CurItemID, NewStoreDealPopupType);
}

void UTwoMinWidget_StoreUI::HideStoreInfoPopup()
{
	ResetHoldDealInput();
	
	StoreDealPopup->HideStorePopup();
	UpdateFocusSlot();
}