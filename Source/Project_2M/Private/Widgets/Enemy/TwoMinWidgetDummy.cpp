// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Enemy/TwoMinWidgetDummy.h"

#include "Compnents/UI/EnemyUIComponent.h"
#include "Components/TextBlock.h"
#include "Interfaces/BaseUIInterface.h"

void UTwoMinWidgetDummy::NativeOnInitialized()
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

void UTwoMinWidgetDummy::InitEnemyIComponent(UEnemyUIComponent* EnemyUIComponent)
{
	EnemyUIComponent->OnEnemyStateChanged.AddUniqueDynamic(this, &UTwoMinWidgetDummy::SetAIStateString);
}

void UTwoMinWidgetDummy::SetAIStateString(const FString& InStateString)
{
	FText StateText = FText::FromString(InStateString);
	AIStateText->SetText(StateText);
}
