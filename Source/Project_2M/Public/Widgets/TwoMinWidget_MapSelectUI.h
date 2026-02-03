// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_MapSelectUI.generated.h"

class UTwoMinWidget_MapSelectSlot;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_MapSelectUI : public UTwoMinWidgetBase
{
	GENERATED_BODY()

public:
	void OnFocusSlot();
	void InitStageButtons();
	
protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* ButtonCanvas;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_MapSelectSlot>> StageButtonSlots;
	
	UPROPERTY()
	int32 CurrentFocusIndex = 0;
};
