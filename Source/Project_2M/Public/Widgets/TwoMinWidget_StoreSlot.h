// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_StoreSlot.generated.h"

class URichTextBlock;
class USizeBox;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_StoreSlot : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void ClearSlot();
	void SetActiveSlot(const FItemData& NewItemData);

protected:
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

private:
	void HighlightStoreSlot(const bool bOn);
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* SlotActiveImageBox;
	
	UPROPERTY(meta=(BindWidget))
	UImage* SlotImageIcon;
	
	UPROPERTY(meta=(BindWidget))
	UImage* SlotFocusImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SlotNameTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* SlotPriceTextBlock;
	
	UPROPERTY()
	int32 ItemID;
	
public:
	FORCEINLINE int32 GetItemID() const { return ItemID; }
	
};
