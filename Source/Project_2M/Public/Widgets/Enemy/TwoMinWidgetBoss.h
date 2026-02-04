// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Enemy/TwoMinWidgetEnemy.h"
#include "TwoMinWidgetBoss.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidgetBoss : public UTwoMinWidgetEnemy
{
	GENERATED_BODY()

public:
	UTwoMinWidgetBoss();
	
	virtual void InitEnemyUIComponent(UEnemyUIComponent* EnemyUIComponent) override;
	
	void ShowBossHealthBar(const FString& BossName);
		
private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* BossNameTextBlock;
};
