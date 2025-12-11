// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_InventorySelect.generated.h"

class UTwoMinWidget_SelectSlot;
enum class EInventorySelectType : uint8;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_InventorySelect : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetInventorySelectType(const EInventorySelectType NewInventorySelectType);
	void SetInventorySelectIndex(int32 NextIndex);
	int32 GetInventorySelectIndex() const;
	void MoveToInventorySelectSlot(int32 NextIndex);
	void QuitInventorySelect();
	
	ESelectEventType OnTriggerEvent(int32 CurIndex);
	void ResetAllSelectSlot();
	
protected:
	virtual void NativeOnInitialized() override;
	void InitInventorySelectSlots(const UVerticalBox* VerticalBox, TArray<TObjectPtr<UTwoMinWidget_SelectSlot>>& SlotGroup);
	
private:
	void SetFocusSlot();
	
	UPROPERTY()
	EInventorySelectType InventorySelectType;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* InventoryToConsume;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_SelectSlot>> InventoryToConsumeSlots;
	
	UPROPERTY()
	int32 InventoryToConsumeSlotIndex;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* QuickToConsume;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_SelectSlot>> QuickToConsumeSlots;
	
	UPROPERTY()
	int32 QuickToConsumeSlotIndex;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* InventoryToEquipment;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_SelectSlot>> InventoryToEquipmentSlots;
	
	UPROPERTY()
	int32 InventoryToEquipmentSlotIndex;
	
	UPROPERTY()
	bool bIsOpen = false;
	
public:
	FORCEINLINE int32 GetInventoryToConsumeSlotIndex() const { return InventoryToConsumeSlotIndex; }
	FORCEINLINE int32 GetQuickToConsumeSlotIndex() const { return QuickToConsumeSlotIndex; }
	FORCEINLINE int32 GetInventoryToEquipmentSlotIndex() const { return InventoryToEquipmentSlotIndex; }
	
	FORCEINLINE bool IsOpen() const { return bIsOpen; }
};
