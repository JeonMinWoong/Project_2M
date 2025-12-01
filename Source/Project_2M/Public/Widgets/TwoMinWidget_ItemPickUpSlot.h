// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_ItemPickUpSlot.generated.h"

class UImage;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_ItemPickUpSlot : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetItemSlotInfo(const FString& ItemName, int32 ItemCount, UTexture2D* ItemTexture);
	void CopyItemSlotData(FText& OutText, float& OutPlayTime, UTexture2D*& OutItemTexture);
	void SetCopyItemSlotInfo(const FText& CopyText, float CopyPlayTime, UTexture2D* CopyTexture);

protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsActive = false;
	
private:
	void StartRemoveProcess(const float PlayTime = 0);

	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* ItemText;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OnItemPickUp;
	
	UFUNCTION()
	void OnActivateEvent();
	
	UFUNCTION()
	void OnDeactivateEvent();
		
	UPROPERTY()
	FWidgetAnimationDynamicEvent AnimationStartEvent;
	
	UPROPERTY()
	FWidgetAnimationDynamicEvent AnimationEndEvent;
	
public:
	FORCEINLINE bool IsActive() const { return bIsActive; }
	
};
