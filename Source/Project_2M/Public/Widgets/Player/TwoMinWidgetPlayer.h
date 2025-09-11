// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidgetPlayer.generated.h"

class UTextBlock;
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

	UFUNCTION()
	void SetCurrentExperiencePercent(float Percent);

	UFUNCTION()
	void SetCurrentLevelValue(int32 InCurrentLevel);
private:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ExperienceBar;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelText;
};
