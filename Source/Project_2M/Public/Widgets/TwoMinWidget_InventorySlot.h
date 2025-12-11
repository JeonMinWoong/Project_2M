// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_InventorySlot.generated.h"

enum class EInventorySlotType : uint8;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_InventorySlot : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	UTwoMinWidget_InventorySlot();
	
	void InitSlot();
	void ClearSlot();
	virtual void SetInventorySlot(FItemInstance NewItemInstance);
	bool IsRegister();
	virtual void OnRegister(bool bIsRegister, EInventorySlotType SlotType, int32 RegisterCount);
	virtual int32 UnRegister();
	virtual void SetRinkItemID(int32 InventorySlotIndex);
	virtual void InventoryRegister(bool bOn, EItemType ItemType, int32 InventorySlotIndex);
	
protected:
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	UPROPERTY(VisibleAnywhere)
	EInventorySlotType InventorySlotType;

	UPROPERTY()
	FItemInstance ItemInstance;
	
private:
	void HighlightInventorySlot(const bool bOn);
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIconImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemCountText;
	
	UPROPERTY(meta=(BindWidget))
	UImage* FocusImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RegisterText;


public:
	FORCEINLINE FItemInstance GetItemInstance() const { return ItemInstance; }
};
