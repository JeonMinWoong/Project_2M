// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_DefeatStageUI.generated.h"

class UTwoMinWidget_BaseButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_DefeatStageUI : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SettingDefeatStageUI();
	
	UFUNCTION(BlueprintCallable)
	void Anim_OnUpdateDefeatCount();

protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	
private:
	void PlayDefeatStageAnim();
	
	UFUNCTION()
	void PlayDefeatCountAnim();
	
	void FinishDefeatCountAnim();
	bool OnReturnButton();

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CountTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_BaseButton* ReturnButton;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxClearCountTime = 10;
	
	UPROPERTY()
	int32 CurClearCountTime = 0;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OnOpenDefeatStageAnim;
	
	UPROPERTY()
	FWidgetAnimationDynamicEvent CompleteOpenDefeatStageAnimEvent;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OnStartDefeatCountAnim;
	
	UPROPERTY()
	bool bIsGoingVillage = false;
};
