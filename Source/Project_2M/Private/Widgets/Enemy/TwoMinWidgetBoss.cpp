// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Enemy/TwoMinWidgetBoss.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UTwoMinWidgetBoss::UTwoMinWidgetBoss()
{
	bIsWorldWidget = false;
}

void UTwoMinWidgetBoss::InitEnemyUIComponent(UEnemyUIComponent* EnemyUIComponent)
{
	Super::InitEnemyUIComponent(EnemyUIComponent);
	
	BossNameTextBlock->SetVisibility(ESlateVisibility::Hidden);
}

void UTwoMinWidgetBoss::ShowBossHealthBar(const FString& BossName)
{
	HealthBar->SetVisibility(ESlateVisibility::Visible);
	HealthBar->SetPercent(1);
	BossNameTextBlock->SetText(FText::FromString(BossName));
	BossNameTextBlock->SetVisibility(ESlateVisibility::Visible);
}
