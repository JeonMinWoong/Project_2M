// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_ClearStageUI.generated.h"

class UTwoMinWidget_BaseButton;
class URichTextBlock;
class UTwoMinWidget_ClearItemSlot;
class UUniformGridPanel;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_ClearStageUI : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SettingClearStageUI();
	
	UFUNCTION(BlueprintCallable)
	void Anim_OnUpdateClearCount();
	bool OnReturnButton();

protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

private:
	void ShowItemSlots(TArray<FItemEquipmentData> InItemEquipmentList, TArray<FItemConsumeData> InItemConsumeList, 
	TArray<FItemEtcData> InItemEtcList);
	
	void PlayClearStageAnim();
	
	UFUNCTION()
	void PlayClearCountAnim();
	void FinishClearCountAnim();
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ClearStageTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* GainGoldTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GainExpTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CountTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_BaseButton* ReturnButton;
	
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* ClearItemGrid;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_ClearItemSlot>> ClearItemSlots;
	
	UPROPERTY()
	int32 CurItemSlotIndex = 0;
	
	UPROPERTY()
	int32 MaxItemSlotIndex;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxClearCountTime = 10;
	
	UPROPERTY()
	int32 CurClearCountTime = 0;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OnOpenClearStageAnim;
	
	UPROPERTY()
	FWidgetAnimationDynamicEvent CompleteOpenClearStageAnimEvent;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OnStartClearCountAnim;
	
	UPROPERTY()
	bool bIsGoingVillage = false;
};
