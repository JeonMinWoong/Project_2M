// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_ItemPickUpWindow.generated.h"

class UVerticalBox;
class UTwoMinWidget_ItemPickUpSlot;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_ItemPickUpWindow : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void OnItemPickUpSlot(int32 ShowAllItemCount, TArray<FItemPickUpEntry>& ItemList);

	void ShowItemPickUpSlot(const FString& ItemName, int32 ItemCount, UTexture2D* ItemTexture);
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* ItemPickUpList;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_ItemPickUpSlot>> ItemPickUpSlotGroup;
	
	UPROPERTY(VisibleAnywhere)
	int32 CurItemPickUpSlotCount = 0;
	
	UPROPERTY(VisibleAnywhere)
	int32 MaxItemPickUpSlotCount = 0;
	
	UPROPERTY(VisibleAnywhere)
	float SlotTerm = 0.2f;
};
