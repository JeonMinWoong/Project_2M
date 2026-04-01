// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_BaseButton.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_BaseButton : public UTwoMinWidgetBase
{
	GENERATED_BODY()

public:
	void SetLocked(bool Locked);
	void SetKeepSelect(bool bOn);

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UTextBlock* TextBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "KeepSelect")
	bool bIsKeepSelect = false;
	
protected:
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	
private:
	void HighlightFocusSlot(bool bOn);
	
	UPROPERTY(meta=(BindWidget))
	UImage* FocusImage;
	
	UPROPERTY(meta=(BindWidget))
	UImage* SelectImage;
	
	UPROPERTY(meta=(BindWidget))
	UImage* LockImage;

	UPROPERTY()
	bool bIsLocked = false;
	
public:
	FORCEINLINE bool IsLocked() const { return bIsLocked; }
};
