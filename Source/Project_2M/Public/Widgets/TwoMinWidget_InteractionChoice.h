// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_InteractionChoice.generated.h"

class UWrapBox;
class UTwoMinWidget_IC_Slot;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_InteractionChoice : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void InitInteractionChoice(ENPCType InNPCType);
	
protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

private:
	void OnFocusSlot();
	
	UPROPERTY(meta=(BindWidget))
	UWrapBox* ChoiceBox;
		
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_IC_Slot>> ChoiceSlots;
	
	UPROPERTY()
	int32 CurrentFocusIndex = 0;
};
