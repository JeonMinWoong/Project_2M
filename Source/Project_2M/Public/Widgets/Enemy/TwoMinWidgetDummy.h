// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinCharacterWidgetBase.h"
#include "TwoMinWidgetDummy.generated.h"

class UEnemyUIComponent;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidgetDummy : public UTwoMinCharacterWidgetBase
{
	GENERATED_BODY()

public:
	void InitEnemyIComponent(UEnemyUIComponent* EnemyUIComponent);

protected:
	virtual void NativeOnInitialized() override;
	
private:
	UFUNCTION()
	void SetAIStateString(const FString& InStateString);
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AIStateText;

};
