// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Player/TwoMinWidgetPlayer.h"

#include "Compnents/UI/PlayerUIComponent.h"
#include "Components/ProgressBar.h"
#include "Interfaces/BaseUIInterface.h"

void UTwoMinWidgetPlayer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const IBaseUIInterface* BaseUIInterface = Cast<IBaseUIInterface>(GetOwningPlayerPawn()))
	{
		if (UPlayerUIComponent* HeroUIComponent = BaseUIInterface->GetPlayerUIComponent())
		{
			InitPlayerUIComponent(HeroUIComponent);	
		}
	}
}

void UTwoMinWidgetPlayer::InitPlayerUIComponent(UPlayerUIComponent* HeroUIComponent)
{
	HeroUIComponent->OnCurrentHealthChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentHealthPercent);
	HeroUIComponent->OnCurrentStaminaChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentStaminaPercent);
}

void UTwoMinWidgetPlayer::SetCurrentHealthPercent(float Percent)
{
	if (!HealthBar) return;

	HealthBar->SetPercent(Percent);
}

void UTwoMinWidgetPlayer::SetCurrentStaminaPercent(float Percent)
{
	if (!StaminaBar) return;

	StaminaBar->SetPercent(Percent);
}
