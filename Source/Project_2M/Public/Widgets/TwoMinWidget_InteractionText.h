// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_InteractionText.generated.h"

class URichTextBlock;

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_InteractionText : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetInteractionText(const FString& NewInteractionText);

protected:
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	
private:
	UFUNCTION()
	void KeyTypeWidgetCreated(const FString& WidgetID, UUserWidget* Widget);
	
	UPROPERTY(meta=(BindWidget))
	URichTextBlock* TextBlock;

	UPROPERTY()
	bool bIsInteractionTexting = false;
	
public:
	FORCEINLINE void SetInteractionTexting(const bool bOn) { bIsInteractionTexting = bOn; }
	FORCEINLINE bool GetInteractionTexting() const { return bIsInteractionTexting; }
};
