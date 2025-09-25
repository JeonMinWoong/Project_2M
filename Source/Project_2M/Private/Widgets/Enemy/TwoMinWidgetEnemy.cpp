// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Enemy/TwoMinWidgetEnemy.h"

#include "Character/TwoMinEnemyDummy.h"
#include "Compnents/UI/EnemyUIComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/BaseUIInterface.h"

void UTwoMinWidgetEnemy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const IBaseUIInterface* BaseUIInterface = Cast<IBaseUIInterface>(GetOwningPlayerPawn()))
	{
		if (UEnemyUIComponent* EnemyUIComponent = BaseUIInterface->GetEnemyUIComponent())
		{
			InitEnemyIComponent(EnemyUIComponent);	
		}
	}
}

void UTwoMinWidgetEnemy::InitEnemyIComponent(UEnemyUIComponent* EnemyUIComponent)
{
	HealthBar->SetVisibility(ESlateVisibility::Hidden);
	EnemyUIComponent->OnCurrentHealthChanged.AddUniqueDynamic(this, &UTwoMinWidgetEnemy::SetCurrentHealthPercent);
}

void UTwoMinWidgetEnemy::SetCurrentHealthPercent(float Percent)
{
	if (!HealthBar) return;
	
	HealthBar->SetVisibility(ESlateVisibility::Visible);
	HealthBar->SetPercent(Percent);

	if (Percent <= 0.f)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UTwoMinWidgetEnemy::OffHealthBar,
			HealthBarHiddenTime,
			false
		);
	}
}

void UTwoMinWidgetEnemy::OffHealthBar()
{
	HealthBar->SetVisibility(ESlateVisibility::Hidden);
}
