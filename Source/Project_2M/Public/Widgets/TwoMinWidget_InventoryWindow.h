// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_InventoryWindow.generated.h"

enum class EInventoryWindowType : uint8;
class UTwoMinWidget_InventorySlot;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_InventoryWindow : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	UTwoMinWidget_InventoryWindow();
	
	void ClearAllSlots();
	
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(VisibleAnywhere)
	EInventoryWindowType InventoryWindowType; 
	
private:
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* InventoryGrid;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_InventorySlot>> InventorySlots;
	
	UPROPERTY()
	int32 CurInventoryIndex = 0;
	
	UPROPERTY()
	int32 MaxInventoryIndex;
	
	UPROPERTY()
	int32 MaxRowIndex;
	
	UPROPERTY()
	int32 MaxColumnIndex;
	
	UPROPERTY()
	UTwoMinWidget_InventorySlot* CurInventorySlot;
	
public:
	FORCEINLINE TArray<TObjectPtr<UTwoMinWidget_InventorySlot>> GetInventorySlots() { return InventorySlots; }
	FORCEINLINE int32 GetCurInventoryIndex() const { return CurInventoryIndex; }
	FORCEINLINE void SetCurInventoryIndex(int32 NextInventoryIndex) { CurInventoryIndex = NextInventoryIndex; }
	FORCEINLINE int32 GetMaxInventoryIndex() const { return MaxInventoryIndex; }
	FORCEINLINE int32 GetMaxRowIndex() const { return MaxRowIndex; }
	FORCEINLINE int32 GetMaxColumnIndex() const { return MaxColumnIndex; }
};
