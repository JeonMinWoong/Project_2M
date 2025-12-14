// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_GoldNotify.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_GoldNotify : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetGoldNotifyText(const FText& GoldText) const;
	
private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GoldNotifyText;
	
};
