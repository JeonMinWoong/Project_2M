// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_WindowQuickSlot.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_WindowQuickSlot : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetCurrentSlotIndex(int32 InIndex);
	
	void SetQuickSlotRegisterItems(FItemInstance QuickSlotRegisterItem, int32 SlotIndex, bool bIsRegister);
	void ShowRegisterText(int32 SlotIndex);
	void UpdateQuickSlotWidget(int32 SlotIndex);
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY()
	TMap<int32, FItemInstance> QuickSlotRegisterItems;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIconImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemCountText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RegisterText;
	
	UPROPERTY()
	int32 CurrentSlotIndex = 0;
	
	UPROPERTY()
	int32 MinSlotIndex = 0;
	
	UPROPERTY()
	int32 MaxSlotIndex = 2;
	
public:
	FORCEINLINE int32 GetCurrentSlotIndex() { return CurrentSlotIndex; }
	FORCEINLINE int32 GetMaxSlotIndex() { return MaxSlotIndex; }
	FORCEINLINE int32 GetMinSlotIndex() { return MinSlotIndex; }
};
