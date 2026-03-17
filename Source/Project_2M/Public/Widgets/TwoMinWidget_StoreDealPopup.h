// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_StoreDealPopup.generated.h"

class ATwoMinPlayerCharacter;
class UTwoMinWidget_BaseButton;
class URichTextBlock;
class USizeBox;
class UProgressBar;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_StoreDealPopup : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	bool TryStoreDealItemInformation(const int32 CurItemID, EStoreDealPopupType NewStoreDealPopupType);
	void SetModifyDealProcess(const FItemData& ItemData, const int32 ModifyDealCount);
	
	void HideStorePopup();
	void SetSelectButton(const int32 ButtonIndex);
	
private:
	void GetItemInformation(const FItemData& ItemData, FText& OutName, UTexture2D*& OutTexture);
	int32 GetMaxDealValue(const FItemData& ItemData, EItemType DealItemType);
	int32 GetCalculateMaxDealValue(const ATwoMinPlayerCharacter* PlayerCharacter, const FItemData& ItemData, 
		const int32 HaveItemCount);
	
	float GetDealProgressPointPos(const float CurProgressPercent);
	void SetModifyInputItemCountText() const;
	void SetDealButtonText();
	
	UPROPERTY()
	EStoreDealPopupType StoreDealPopupType;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNameText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemTypeText;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DealMinCountText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DealMaxCountText;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* DealProgressBar;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* DealProgressPoint;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DealDecreaseInputText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DealIncreaseInputText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DealCountText;
	
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* DealPriceText;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_BaseButton* StorePopupButton_1;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_BaseButton* StorePopupButton_2;
	
	UPROPERTY()
	bool bIsOpenPopup = false;
	
	UPROPERTY()
	int32 MinDealValue = 1;
	
	UPROPERTY()
	int32 MaxDealValue = 1;
	
	UPROPERTY()
	int32 CurDealValue = 1;
	
	UPROPERTY()
	EStoreDealTextType StoreDealTextType = EStoreDealTextType::None;
	
	UPROPERTY()
	int32 CurButtonIndex;
	
public:
	FORCEINLINE bool IsOpenPopup() const { return bIsOpenPopup; }
	FORCEINLINE EStoreDealPopupType GetStoreDealPopupType() const { return StoreDealPopupType; }
	FORCEINLINE EStoreDealTextType GetStoreDealTextType() const { return StoreDealTextType; }
	FORCEINLINE int32 GetCurDealValue() const { return CurDealValue; }
	FORCEINLINE int32 GetCurButtonIndex() const { return CurButtonIndex; }
};
