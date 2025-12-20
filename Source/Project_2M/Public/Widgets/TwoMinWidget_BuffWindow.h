// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_BuffWindow.generated.h"

struct FItemInstance;
class UTwoMinWidget_BuffSlot;
class UGridPanel;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_BuffWindow : public UTwoMinWidgetBase
{
	GENERATED_BODY()

public:
	void SetBuffSlot(const int32 ItemID);
	void SortBuffSlots(int32 OffSlotIndex);
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UGridPanel* BuffGrid;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_BuffSlot>> BuffSlots;
	
	UPROPERTY()
	int32 CurSlotIndex = 0;
	
	UPROPERTY()
	int32 MaxSlotIndex = 0;
};
