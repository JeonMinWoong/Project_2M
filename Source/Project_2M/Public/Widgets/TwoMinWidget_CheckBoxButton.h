// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_CheckBoxButton.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_CheckBoxButton : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetLocked(bool Locked);
	void SetCheckBox(bool bOn);
	
protected:
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	
private:
	void HighlightFocusSlot(bool bOn);
	
	UPROPERTY(meta=(BindWidget))
	UImage* FocusImage;
	
	UPROPERTY(meta=(BindWidget))
	UImage* CheckImage;
	
	UPROPERTY(meta=(BindWidget))
	UImage* LockImage;
	
	UPROPERTY()
	bool bIsLocked = false;
	
	UPROPERTY()
	bool bIsCheckBox = false;
};
