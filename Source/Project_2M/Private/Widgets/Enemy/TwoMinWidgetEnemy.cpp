// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Enemy/TwoMinWidgetEnemy.h"

#include "Character/TwoMinEnemyDummy.h"
#include "Compnents/UI/EnemyUIComponent.h"
#include "Components/ProgressBar.h"
#include "Interfaces/BaseUIInterface.h"

UTwoMinWidgetEnemy::UTwoMinWidgetEnemy()
{
	bIsWorldWidget = true;
}

void UTwoMinWidgetEnemy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const IBaseUIInterface* BaseUIInterface = Cast<IBaseUIInterface>(GetOwningPlayerPawn()))
	{
		if (UEnemyUIComponent* EnemyUIComponent = BaseUIInterface->GetEnemyUIComponent())
		{
			InitEnemyUIComponent(EnemyUIComponent);	
		}
	}
}

void UTwoMinWidgetEnemy::InitEnemyUIComponent(UEnemyUIComponent* EnemyUIComponent)
{
	HealthBar->SetVisibility(ESlateVisibility::Hidden);
	EnemyUIComponent->OnCurrentHealthChanged.AddUniqueDynamic(this, &UTwoMinWidgetEnemy::SetCurrentHealthPercent);
}

void UTwoMinWidgetEnemy::HideWorldHealthBar()
{
	if (!HealthBar) return;
	
	HealthBar->SetVisibility(ESlateVisibility::Hidden);
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
	if (!bIsWorldWidget)
	{
		RemoveFromParent();
	}
	else
	{
		HealthBar->SetVisibility(ESlateVisibility::Hidden);	
	}
}
