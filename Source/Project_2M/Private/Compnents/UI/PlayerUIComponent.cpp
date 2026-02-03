// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/UI/PlayerUIComponent.h"

#include "Blueprint/UserWidget.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/TwoMinWidget_MapSelectUI.h"

void UPlayerUIComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!MapSelectWidgetClass)
	{
		return;
	}
	
	if (!MapSelectUI)
	{
		MapSelectUI = CreateWidget<UTwoMinWidget_MapSelectUI>(GetWorld(), MapSelectWidgetClass);	
	}
}

void UPlayerUIComponent::OpenMapSelectWidget(const ATwoMinPlayerCharacter* PlayerCharacter, const bool bIsOpenMapSelectWidget)
{
	if (!MapSelectWidgetClass) return;
	
	if (!PlayerCharacter) return;
	
	ATwoMinPlayerController* PC = PlayerCharacter->GetPlayerController();
	if (!PC) return;
	
	float TimeDilation;
	if (bIsOpenMapSelectWidget)
	{
		TimeDilation = 0;
		MapSelectUI->AddToViewport();
		MapSelectUI->InitStageButtons();
		MapSelectUI->OnFocusSlot();
	}
	else
	{
		TimeDilation = 1.f;
		MapSelectUI->RemoveFromParent();
	}
	
	bIsMapSelectWidgetOpen = bIsOpenMapSelectWidget;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}
