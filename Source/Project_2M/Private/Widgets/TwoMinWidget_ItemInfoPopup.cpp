// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_ItemInfoPopup.h"

#include "TwoMinFunctionLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/ItemDataManager.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinWidget_ItemInfoPopup::SetItemInformation(const FItemInstance ItemInstance)
{
	bIsPopupOpen = true;
	
	FText ItemName;
	UTexture2D* ItemTexture = nullptr;
	FText ItemType;
	FText ItemStatus;
	FText ItemInfo;
	
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	EItemType CurItemType = UTwoMinFunctionLibrary::GetItemType(ItemInstance.ItemID);
	if (CurItemType == EItemType::Equipment)
	{
		FItemEquipmentData EquipmentData = GI->ItemDataManager->GetItemEquipmentData(ItemInstance.ItemID);
		GetItemInformation(EquipmentData, ItemName, ItemType, ItemTexture, ItemStatus, ItemInfo);
	}
	else if (CurItemType == EItemType::Consume)
	{
		FItemConsumeData ConsumeData = GI->ItemDataManager->GetItemConsumeData(ItemInstance.ItemID);
		GetItemInformation(ConsumeData, ItemName, ItemType, ItemTexture, ItemStatus, ItemInfo);
	}
	else if (CurItemType == EItemType::Etc)
	{
		FItemEtcData EtcData = GI->ItemDataManager->GetItemEtcData(ItemInstance.ItemID);
		GetItemInformation(EtcData, ItemName, ItemType, ItemTexture, ItemStatus, ItemInfo);
	}
	
	ItemNameText->SetText(ItemName);
	ItemTypeText->SetText(ItemType);
	ItemImage->SetBrushFromTexture(ItemTexture);
	ItemStatusText->SetText(ItemStatus);
	ItemInfoText->SetText(ItemInfo);
	SetFocus();
}

void UTwoMinWidget_ItemInfoPopup::GetItemInformation(const FItemEquipmentData& EquipmentData, FText& OutName, FText& OutType,
	UTexture2D*& OutTexture, FText& OutStatus, FText& OutInfo)
{
	OutName = FText::FromString(EquipmentData.ItemDataBase.ItemName);
	OutType = FText::FromString(TEXT("장비"));
	OutTexture = EquipmentData.ItemDataBase.ItemTexture;
	
	FString ToTalStr = TEXT("");
	if (EquipmentData.AttackPower > 0)
	{
		FString Str = FString::Printf(TEXT("공격력 + %d"), EquipmentData.AttackPower);
		ToTalStr = Str;
	}
	
	if (EquipmentData.DefensePower > 0)
	{
		FString Str = FString::Printf(TEXT("방어력 + %d"), EquipmentData.DefensePower);
		if (ToTalStr.IsEmpty())
		{
			ToTalStr = Str;
		}
		else
		{
			ToTalStr += TEXT("\n") + Str;
		}
	}
		
	OutStatus = FText::FromString(ToTalStr);	
	OutInfo = FText::FromString(EquipmentData.ItemDataBase.ItemInformation);
}

void UTwoMinWidget_ItemInfoPopup::GetItemInformation(const FItemConsumeData& ConsumeData, FText& OutName,
	FText& OutType, UTexture2D*& OutTexture, FText& OutStatus, FText& OutInfo)
{
	OutName = FText::FromString(ConsumeData.ItemDataBase.ItemName);
	OutType = FText::FromString(TEXT("소비"));
	OutTexture = ConsumeData.ItemDataBase.ItemTexture;
		
	if (ConsumeData.ConsumeType == EConsumeType::Heal)
	{
		int32 IntValue = static_cast<int32>(ConsumeData.ConsumePowerGroup[0] * 100);
		FString Str = FString::Printf(TEXT("체력 회복 + %d%%"), IntValue);
		OutStatus = FText::FromString(Str);	
	}
	else if (ConsumeData.ConsumeType == EConsumeType::Buff)
	{
		FString ToTalStr = TEXT("");
		if (ConsumeData.ConsumePowerGroup[0] > 0.f)
		{
			int32 IntValue = static_cast<int32>(ConsumeData.ConsumePowerGroup[0] * 100);
			FString Str = FString::Printf(TEXT("공격력 + %d%%"), IntValue);
			ToTalStr += Str;
		}
		
		if (ConsumeData.ConsumePowerGroup[1] > 0.f)
		{
			int32 IntValue = static_cast<int32>(ConsumeData.ConsumePowerGroup[1] * 100);
			FString Str2 = FString::Printf(TEXT("\n방어력 + %d%%"), IntValue);
			ToTalStr += Str2;
		}
		
		OutStatus = FText::FromString(ToTalStr);	
	}
		
	OutInfo = FText::FromString(ConsumeData.ItemDataBase.ItemInformation);
}

void UTwoMinWidget_ItemInfoPopup::GetItemInformation(const FItemEtcData& EtcData, FText& OutName, FText& OutType,
	UTexture2D*& OutTexture, FText& OutStatus, FText& OutInfo)
{
	OutName = FText::FromString(EtcData.ItemDataBase.ItemName);
	OutType = FText::FromString(TEXT("기타"));
	OutTexture = EtcData.ItemDataBase.ItemTexture;
	OutStatus = FText::FromString(TEXT("-"));
	OutInfo = FText::FromString(EtcData.ItemDataBase.ItemInformation);
}

void UTwoMinWidget_ItemInfoPopup::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		SetFocus();
	}
}