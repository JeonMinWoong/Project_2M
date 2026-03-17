// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_ItemInfoPopup.h"

#include "TwoMinFunctionLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/ItemDataManager.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinWidget_ItemInfoPopup::SetItemInformation(const int32 CurItemID)
{
	bIsPopupOpen = true;
	
	FText ItemName;
	UTexture2D* ItemTexture = nullptr;
	FText ItemType;
	FText ItemStatus;
	FText ItemInfo;
	
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	EItemType CurItemType = UTwoMinFunctionLibrary::GetItemType(CurItemID);
	if (CurItemType == EItemType::Equipment)
	{
		FItemEquipmentData EquipmentData = GI->ItemDataManager->GetItemEquipmentData(CurItemID);
		GetItemInformation(EquipmentData, ItemName, ItemType, ItemTexture, ItemStatus, ItemInfo);
	}
	else if (CurItemType == EItemType::Consume)
	{
		FItemConsumeData ConsumeData = GI->ItemDataManager->GetItemConsumeData(CurItemID);
		GetItemInformation(ConsumeData, ItemName, ItemType, ItemTexture, ItemStatus, ItemInfo);
	}
	else if (CurItemType == EItemType::Etc)
	{
		FItemEtcData EtcData = GI->ItemDataManager->GetItemEtcData(CurItemID);
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
	if (EquipmentData.EquipmentPower.Contains(EStatusType::Attack))
	{
		FString Str = FString::Printf(TEXT("공격력 + %d"), EquipmentData.EquipmentPower[EStatusType::Attack]);
		ToTalStr = Str;
	}
	
	if (EquipmentData.EquipmentPower.Contains(EStatusType::Defense))
	{
		FString Str = FString::Printf(TEXT("방어력 + %d"), EquipmentData.EquipmentPower[EStatusType::Defense]);
		if (ToTalStr.IsEmpty())
		{
			ToTalStr = Str;
		}
		else
		{
			ToTalStr += TEXT("\n") + Str;
		}
	}
	
	if (EquipmentData.EquipmentPower.Contains(EStatusType::MaxHealth))
	{
		FString Str = FString::Printf(TEXT("체력 + %d"), EquipmentData.EquipmentPower[EStatusType::MaxHealth]);
		if (ToTalStr.IsEmpty())
		{
			ToTalStr = Str;
		}
		else
		{
			ToTalStr += TEXT("\n") + Str;
		}
	}
	
	if (EquipmentData.EquipmentPower.Contains(EStatusType::MaxStamina))
	{
		FString Str = FString::Printf(TEXT("지구력 + %d"), EquipmentData.EquipmentPower[EStatusType::MaxStamina]);
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
		if (ConsumeData.ConsumePower.Contains(EStatusType::HpHeal))
		{
			int32 IntValue = static_cast<int32>(ConsumeData.ConsumePower[EStatusType::HpHeal] * 100);
			FString Str = FString::Printf(TEXT("체력 회복 + %d%%"), IntValue);
			OutStatus = FText::FromString(Str);	
		}
	}
	else if (ConsumeData.ConsumeType == EConsumeType::Buff)
	{
		FString ToTalStr = TEXT("");
		if (ConsumeData.ConsumePower.Contains(EStatusType::Attack))
		{
			int32 IntValue = static_cast<int32>(ConsumeData.ConsumePower[EStatusType::Attack] * 100);
			FString Str = FString::Printf(TEXT("공격력 + %d%%"), IntValue);
			ToTalStr += Str;
		}
		
		if (ConsumeData.ConsumePower.Contains(EStatusType::Defense))
		{
			int32 IntValue = static_cast<int32>(ConsumeData.ConsumePower[EStatusType::Defense] * 100);
			FString Str = FString::Printf(TEXT("방어력 + %d%%"), IntValue);
			if (ToTalStr.IsEmpty())
			{
				ToTalStr = Str;
			}
			else
			{
				ToTalStr += TEXT("\n") + Str;
			}
		}
		
		if (ConsumeData.ConsumePower.Contains(EStatusType::MaxHealth))
		{
			int32 IntValue = static_cast<int32>(ConsumeData.ConsumePower[EStatusType::MaxHealth] * 100);
			FString Str = FString::Printf(TEXT("체력 + %d%%"), IntValue);
			if (ToTalStr.IsEmpty())
			{
				ToTalStr = Str;
			}
			else
			{
				ToTalStr += TEXT("\n") + Str;
			}
		}
		
		if (ConsumeData.ConsumePower.Contains(EStatusType::MaxStamina))
		{
			int32 IntValue = static_cast<int32>(ConsumeData.ConsumePower[EStatusType::MaxStamina] * 100);
			FString Str = FString::Printf(TEXT("지구력 + %d%%"), IntValue);
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