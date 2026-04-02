// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_CustomProgressbar.generated.h"

class USizeBox;
class UTextBlock;
class UImage;
class UTwoMinWidget_CheckBoxButton;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_CustomProgressbar : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void InitProgressbar(float InitPercent, bool bIsMute);
	void SetPercent(const float InPercent) const;
	void SetModifyInputText() const;
	
	void SetFocusMuteButton() const;
	void SetEnableInputText(bool bOn) const;
	void SetCheckMuteButton(bool bOn) const;
protected:
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

private:
	void HighlightFocusSlot(bool bOn) const;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* ProgressPoint;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ProgressPointImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DecreaseInputText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* IncreaseInputText;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_CheckBoxButton* MuteButton;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PercentText;
	
	UPROPERTY(meta=(BindWidget))
	UImage* DisableImage;
	
};
