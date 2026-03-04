// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "EnemyUIComponent.generated.h"

class UTwoMinWidgetBoss;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UEnemyUIComponent : public UBaseUIComponent
{
	GENERATED_BODY()

public:
	void ShowBossHealthBar(const FString& BossName);
	void HideBossHealthBar();
	
	FOnStringChangeDelegate OnEnemyStateChanged;
	
private:
	UPROPERTY(EditDefaultsOnly, Category= "CharacterInfo|BossHealthBar")
	TSubclassOf<UTwoMinWidgetBoss> BossHealthBarWidgetClass;
	
	UPROPERTY()
	UTwoMinWidgetBoss* BossHealthBarWidget;
};
