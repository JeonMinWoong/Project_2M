// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidgetEnemy.generated.h"

class UProgressBar;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidgetEnemy : public UTwoMinWidgetBase
{
	GENERATED_BODY()

public:
	void InitEnemyIComponent(UEnemyUIComponent* EnemyUIComponent);
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void SetCurrentHealthPercent(float Percent) override;

	UFUNCTION()
	void OffHealthBar();

	UPROPERTY(EditDefaultsOnly)
	float HealthBarHiddenTime = 3.f;
	
private:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;
};
