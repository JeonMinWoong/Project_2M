// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinCharacterWidgetBase.h"
#include "TwoMinWidgetEnemy.generated.h"

class UTextBlock;
class UProgressBar;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidgetEnemy : public UTwoMinCharacterWidgetBase
{
	GENERATED_BODY()

public:
	UTwoMinWidgetEnemy();
	
	virtual void InitEnemyUIComponent(UEnemyUIComponent* EnemyUIComponent);
	void HideWorldHealthBar();
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void SetCurrentHealthPercent(float Percent) override;
	
	UFUNCTION()
	void OffHealthBar();

	UPROPERTY(EditDefaultsOnly)
	float HealthBarHiddenTime = 3.f;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY()
	bool bIsWorldWidget;
};
