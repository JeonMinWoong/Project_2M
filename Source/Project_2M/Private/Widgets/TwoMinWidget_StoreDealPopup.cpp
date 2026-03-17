// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_StoreDealPopup.h"

#include "TwoMinFunctionLibrary.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/InventoryComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/RichTextBlock.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/ItemDataManager.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidget_BaseButton.h"

bool UTwoMinWidget_StoreDealPopup::TryStoreDealItemInformation(const int32 CurItemID, EStoreDealPopupType NewStoreDealPopupType)
{
	SetVisibility(ESlateVisibility::Visible);
	
	bIsOpenPopup = true;
	StoreDealPopupType = NewStoreDealPopupType;;
	MinDealValue = 1;
	MaxDealValue = 1;
	
	FText ItemName;
	UTexture2D* ItemTexture = nullptr;
	FText ItemType;
	FItemData ItemData;
	
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	EItemType CurItemType = UTwoMinFunctionLibrary::GetItemType(CurItemID);
	if (CurItemType == EItemType::Equipment)
	{
		FItemEquipmentData EquipmentData = GI->ItemDataManager->GetItemEquipmentData(CurItemID);
		ItemData = EquipmentData.ItemDataBase;
		
		GetItemInformation(ItemData, ItemName, ItemTexture);
		ItemType = FText::FromString(TEXT("장비"));
	}
	else if (CurItemType == EItemType::Consume)
	{
		FItemConsumeData ConsumeData = GI->ItemDataManager->GetItemConsumeData(CurItemID);
		ItemData = ConsumeData.ItemDataBase;
		
		GetItemInformation(ItemData, ItemName, ItemTexture);
		ItemType = FText::FromString(TEXT("소비"));
	}
	else if (CurItemType == EItemType::Etc)
	{
		FItemEtcData EtcData = GI->ItemDataManager->GetItemEtcData(CurItemID);
		ItemData = EtcData.ItemDataBase;
		
		GetItemInformation(ItemData, ItemName, ItemTexture);
		ItemType = FText::FromString(TEXT("기타"));
	}
	
	MaxDealValue = GetMaxDealValue(ItemData, CurItemType);
	if (MaxDealValue <= 0)
	{
		return false;
	}
	
	ItemNameText->SetText(ItemName);
	ItemTypeText->SetText(ItemType);
	ItemImage->SetBrushFromTexture(ItemTexture);
	DealMinCountText->SetText(FText::FromString(FString::FromInt(MinDealValue)));
	DealMaxCountText->SetText(FText::FromString(FString::FromInt(MaxDealValue)));
	
	SetModifyInputItemCountText();
	SetModifyDealProcess(ItemData, 0);
	SetDealButtonText();
	
	return true;
}

void UTwoMinWidget_StoreDealPopup::SetModifyDealProcess(const FItemData& ItemData, const int32 ModifyDealCount)
{
	FString DealCountStr;
	int32 DealPriceValue = 0;
	
	int32 NewDealCount = CurDealValue + ModifyDealCount;
	if (NewDealCount < MinDealValue)
	{
		NewDealCount = MinDealValue;
	}
	
	if (NewDealCount > MaxDealValue)
	{
		NewDealCount = MaxDealValue;
	}

	CurDealValue = NewDealCount;
	
	switch (StoreDealPopupType) {
	case EStoreDealPopupType::Sell:
		DealCountStr = FString::Printf(TEXT("판매 "));
		DealPriceValue = ItemData.SellPrice * NewDealCount;
		break;
	case EStoreDealPopupType::Buy:
		DealCountStr = FString::Printf(TEXT("구매 "));
		DealPriceValue = ItemData.BuyPrice * NewDealCount;
		break;
	}

	const FString DealItemCount = DealCountStr + FString::Printf(TEXT("수량 : %d"), NewDealCount); 
	DealCountText->SetText(FText::FromString(DealItemCount));
	const FString DealPriceStr = FString::Printf(TEXT("<StoreGold>%d</><img id=\"ClearGoldImage\"/>"), DealPriceValue);
	DealPriceText->SetText(FText::FromString(DealPriceStr));
	
	const float ProgressPercent = static_cast<float>(CurDealValue) / MaxDealValue;
	DealProgressBar->SetPercent(ProgressPercent);
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DealProgressPoint->Slot))
	{
		CanvasSlot->SetPosition(FVector2D(GetDealProgressPointPos(ProgressPercent), 0));
	}
}

void UTwoMinWidget_StoreDealPopup::HideStorePopup()
{
	SetVisibility(ESlateVisibility::Hidden);
	
	bIsOpenPopup = false;
	CurDealValue = 1;
	MinDealValue = 1;
	MaxDealValue = 1;
	StoreDealTextType = EStoreDealTextType::None;
}

void UTwoMinWidget_StoreDealPopup::SetSelectButton(const int32 ButtonIndex)
{
	CurButtonIndex = ButtonIndex;
	if (CurButtonIndex == 0)
	{
		StorePopupButton_1->SetFocus();
	}
	else
	{
		StorePopupButton_2->SetFocus();
	}
}

void UTwoMinWidget_StoreDealPopup::GetItemInformation(const FItemData& ItemData, FText& OutName, UTexture2D*& OutTexture)
{
	OutName = FText::FromString(ItemData.ItemName);
	OutTexture = ItemData.ItemTexture;
}

int32 UTwoMinWidget_StoreDealPopup::GetMaxDealValue(const FItemData& ItemData, EItemType DealItemType)
{
	bool bIsSell = StoreDealPopupType == EStoreDealPopupType::Sell;
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return 0;
	
	UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!InventoryComponent) return 0;
	
	switch (DealItemType) {
	case EItemType::None:
	case EItemType::Unknown:
		return 0;
	case EItemType::Equipment:
	case EItemType::Consume:
	case EItemType::Etc:
		int32 HaveItemCount = InventoryComponent->GetHaveSingleItemCount(ItemData.ItemID);
		return bIsSell ? HaveItemCount : GetCalculateMaxDealValue(PlayerCharacter, ItemData, HaveItemCount);
	}
	
	return 0;
}

int32 UTwoMinWidget_StoreDealPopup::GetCalculateMaxDealValue(const ATwoMinPlayerCharacter* PlayerCharacter, const FItemData& ItemData,
	const int32 HaveItemCount)
{
	const UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return 0;
	
	const int32 RemainBuyCount = ItemData.MaxCount - HaveItemCount;
	if (RemainBuyCount <= 0)
	{
		StoreDealTextType = EStoreDealTextType::Buy_Failure_FullAmount;
		return 0;
	}
	
	const int32 HaveGold = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentGoldAttribute());
	const int32 PossibleBuyCount = HaveGold / ItemData.BuyPrice;
	if (PossibleBuyCount <= 0)
	{
		StoreDealTextType = EStoreDealTextType::Buy_Failure_LackGold;
		return 0;
	}
	
	return RemainBuyCount > PossibleBuyCount ? PossibleBuyCount : RemainBuyCount;
}

float UTwoMinWidget_StoreDealPopup::GetDealProgressPointPos(const float CurProgressPercent)
{
	// 0.5 = 0
	// 0 = -100
	// 1 = 100
	return FMath::Lerp(-100, 100, CurProgressPercent);
}

void UTwoMinWidget_StoreDealPopup::SetModifyInputItemCountText() const
{
	const bool bIsUsingGamePad = UTwoMinFunctionLibrary::IsUsingGamePad(GetWorld(), GetOwningPlayer()->GetPlatformUserId());
	const FString DecreaseKeyText = bIsUsingGamePad ? TEXT("LB") : TEXT("Q");
	const FString IncreaseKeyText = bIsUsingGamePad ? TEXT("RB") : TEXT("E");
	
	if (DealDecreaseInputText->GetText().ToString() == DecreaseKeyText) return;
	if (DealIncreaseInputText->GetText().ToString() == IncreaseKeyText) return;
	
	DealDecreaseInputText->SetText(FText::FromString(DecreaseKeyText));
	DealIncreaseInputText->SetText(FText::FromString(IncreaseKeyText));
}

void UTwoMinWidget_StoreDealPopup::SetDealButtonText()
{
	switch (StoreDealPopupType) {
	case EStoreDealPopupType::Sell:
		StorePopupButton_1->TextBox->SetText(FText::FromString(TEXT("판매")));
		break;
	case EStoreDealPopupType::Buy:
		StorePopupButton_1->TextBox->SetText(FText::FromString(TEXT("구매")));
		break;
	}
	
	StorePopupButton_1->SetIsFocusable(true);
	StorePopupButton_2->SetIsFocusable(true);
	
	SetSelectButton(0);
}
