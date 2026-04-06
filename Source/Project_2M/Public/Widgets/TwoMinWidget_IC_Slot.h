// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_IC_Slot.generated.h"

class UTwoMinWidget_KeyType;
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
	virtual void OnInputDeviceChanged(bool bIsGamePad) override;
	
private:
	void OnEnable();
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_KeyType* FocusKey;
	
	bool bLocked = false;
	
public:
	FORCEINLINE bool IsLocked() const { return bLocked; }
};
