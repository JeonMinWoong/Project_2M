// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_StoreWindow.generated.h"

class UScrollBox;
class UTwoMinWidget_StoreSlot;

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_StoreWindow : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void ResettingStoreList(const TSet<int32>& StoreList);
	void SetFocusSlot(const int32 NewCurStoreIndex);
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta=(BindWidget))
	UScrollBox* StoreScrollBox;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_StoreSlot>> StoreSlots;
	
	UPROPERTY()
	int32 CurStoreIndex = 0;
	
	UPROPERTY()
	int32 MaxStoreIndex;
	
	UPROPERTY()
	int32 FixedStoreSlotCount = 9;
	
public:
	FORCEINLINE void SetCurStoreIndex(const int32 NewStoreIndex) { CurStoreIndex = NewStoreIndex; };
	FORCEINLINE int32 GetCurStoreIndex() const { return CurStoreIndex; }
	FORCEINLINE int32 GetMaxStoreIndex() const { return MaxStoreIndex; }
	FORCEINLINE bool IsEmptySlotIndex(const int32 NewCurStoreIndex) const
	{
		return StoreSlots.IsValidIndex(NewCurStoreIndex) == false;
	};
	
	FORCEINLINE TArray<TObjectPtr<UTwoMinWidget_StoreSlot>> GetStoreSlots() const { return StoreSlots; };
};
