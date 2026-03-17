// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_EndGameUI.generated.h"

class UTwoMinWidget_BaseButton;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_EndGameUI : public UTwoMinWidgetBase
{
	GENERATED_BODY()

public:
	void InitEndGameUI();
	void ResetEndGameUI();

protected:
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_BaseButton* EndGameButton_1;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_BaseButton* EndGameButton_2;
	
	UPROPERTY()
	int32 CurrentFocusIndex;
	
};
