// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_TitleUI.generated.h"

class UTwoMinWidget_BaseButton;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_TitleUI : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void OnFocusSlot();
	
protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* TitleMenuGridPanel;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_BaseButton>> MenuButtonSlots;
	
	UPROPERTY()
	int32 CurrentFocusIndex = 0;
	
	UPROPERTY()
	bool bIsLockInputKey = false;
	
};
