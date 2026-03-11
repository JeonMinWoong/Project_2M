// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_IC_Slot.generated.h"

class USizeBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_IC_Slot : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetFocusSlot(bool bOn);
	void SetLocked(bool bOn);
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UTextBlock* TextBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENPCType SlotNPCType;
	
protected:
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	
private:
	UPROPERTY(meta=(BindWidget))
	USizeBox* FocusBox;
	
	bool bLocked = false;
	
public:
	FORCEINLINE bool IsLocked() const { return bLocked; }
};
