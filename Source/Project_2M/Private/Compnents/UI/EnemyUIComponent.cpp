// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/UI/EnemyUIComponent.h"

#include "Widgets/Enemy/TwoMinWidgetBoss.h"

void UEnemyUIComponent::ShowBossHealthBar(const FString& BossName)
{
	if (!BossHealthBarWidgetClass) return;
		
	if (!BossHealthBarWidget)
	{
		BossHealthBarWidget = Cast<UTwoMinWidgetBoss>(CreateWidget(GetWorld(),  BossHealthBarWidgetClass));	
	}
		
	BossHealthBarWidget->AddToViewport();
	BossHealthBarWidget->InitEnemyUIComponent(this);
	BossHealthBarWidget->ShowBossHealthBar(BossName);
}
