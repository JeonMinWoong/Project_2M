// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_BaseButton.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_BaseButton : public UTwoMinWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	
private:
	void HighlightFocusSlot(bool bOn);
	
	UPROPERTY(meta=(BindWidget))
	UImage* FocusImage;
};
