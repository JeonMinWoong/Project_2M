// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_StoreDealText.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_StoreDealText : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetTextValue(const FString& DealSuccessText);
	
private:
	void StartFadeOutAnim();
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextBlock;
	
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* OnStartFadeOutAnim;
	
};
