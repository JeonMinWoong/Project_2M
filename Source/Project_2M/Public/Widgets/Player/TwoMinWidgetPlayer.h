// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidgetPlayer.generated.h"

class UProgressBar;
class UPlayerUIComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidgetPlayer : public UTwoMinWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	void InitPlayerUIComponent(UPlayerUIComponent* HeroUIComponent);
	
	virtual void SetCurrentHealthPercent(float Percent) override;

	UFUNCTION()
	void SetCurrentStaminaPercent(float Percent);
private:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar;
};
