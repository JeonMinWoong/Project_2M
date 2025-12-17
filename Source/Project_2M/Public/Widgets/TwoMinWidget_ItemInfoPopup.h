// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_ItemInfoPopup.generated.h"

class UButton;
struct FItemConsumeData;
struct FItemEquipmentData;
struct FItemInstance;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_ItemInfoPopup : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetItemInformation(const FItemInstance ItemInstance);
	
protected:
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

private:
	void GetItemInformation(const FItemEquipmentData& EquipmentData, FText& OutName, FText& OutType, 
		UTexture2D*& OutTexture, FText& OutStatus, FText& OutInfo);
	
	void GetItemInformation(const FItemConsumeData& ConsumeData, FText& OutName, FText& OutType, 
		UTexture2D*& OutTexture, FText& OutStatus, FText& OutInfo);
	
	void GetItemInformation(const FItemEtcData& EtcData, FText& OutName, FText& OutType, 
		UTexture2D*& OutTexture, FText& OutStatus, FText& OutInfo);

	void HighlightButton(bool bOn);
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNameText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemTypeText;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemStatusText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemInfoText;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ButtonImage;
	
	UPROPERTY()
	bool bIsPopupOpen;
	
public:
	FORCEINLINE bool IsPopupOpen() const { return bIsPopupOpen; }
	FORCEINLINE void HidePopup() { bIsPopupOpen = false; }
	
};
