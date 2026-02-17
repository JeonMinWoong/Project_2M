// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_ClearItemSlot.generated.h"

class UTextBlock;
class UImage;
struct FItemData;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_ClearItemSlot : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void InitSlot();
	void SetClearItemSlotInfo(FItemData ItemData);
	
private:
	FText GetTruncateItemName(const FString& ItemName) const;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIconImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemCountTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* ItemNameTextBlock;
	
	UPROPERTY(VisibleDefaultsOnly)
	int32 MaxItemNameLength = 8;
};
