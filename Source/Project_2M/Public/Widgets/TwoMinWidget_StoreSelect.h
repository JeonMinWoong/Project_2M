// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_StoreSelect.generated.h"

class UTwoMinWidget_SelectSlot;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_StoreSelect : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetStoreSelectType(EStoreSelectType NewStoreSelectType);
	void SetStoreSelectIndex(int32 NextIndex);
	int32 GetStoreSelectIndex() const;
	void OnFocusStoreSelectSlot(int32 NextIndex);
	void QuitInventorySelect();
	ESelectEventType OnTriggerEvent(int32 CurIndex);

protected:
	virtual void NativeOnInitialized() override;
	
private:
	void InitStoreSelectSlots(const UVerticalBox* VerticalBox, TArray<TObjectPtr<UTwoMinWidget_SelectSlot>>& SlotGroup);
	
	int32 GetMaxSelectSlotIndex() const;
	
	void ResetAllSelectSlot();
	void OnFocusFirstSlot();

	UPROPERTY()
	EStoreSelectType StoreSelectType;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* InventoryToStore;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_SelectSlot>> InventoryToStoreSlots;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* StoreToInventory;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_SelectSlot>> StoreToInventorySlots;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* NoDeal;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_SelectSlot>> NoDealSlots;
	
	UPROPERTY()
	int32 CurSelectSlotIndex;
	
	UPROPERTY()
	bool bIsOpen = false;
	
public:
	FORCEINLINE bool IsOpen() const { return bIsOpen; }
};
