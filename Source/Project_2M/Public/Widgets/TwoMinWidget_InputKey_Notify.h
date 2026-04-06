// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_InputKey_Notify.generated.h"

class UTextBlock;
class UTwoMinWidget_KeyType;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_InputKey_Notify : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void OnEnable(bool bOn);

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UTextBlock* TextBox;

protected:
	virtual void OnInputDeviceChanged(bool bIsGamePad) override;

private:
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_KeyType* KeyType;
};
