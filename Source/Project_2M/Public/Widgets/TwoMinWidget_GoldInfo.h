// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_GoldInfo.generated.h"

class UTwoMinWidget_GoldNotify;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_GoldInfo : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetGoldText(int32 InGoldAmount);
	void SetGoldNotifyText(int32 GainAmount);
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GoldTextBlock;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_GoldNotify* GoldNotify;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* WAnim_GainGold;
};
