// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/UI/PlayerUIComponent.h"

#include "Blueprint/UserWidget.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "System/InteractionActor_NPC.h"
#include "Widgets/TwoMinWidget_EndGameUI.h"
#include "Widgets/TwoMinWidget_MapSelectUI.h"
#include "Widgets/TwoMinWidget_StoreUI.h"

void UPlayerUIComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitMapSelectUI();
	InitStoreUI();
	InitEndGameUI();
}

void UPlayerUIComponent::InitMapSelectUI()
{
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

void UPlayerUIComponent::InitStoreUI()
{
	if (!StoreWidgetClass)
	{
		return;
	}
	
	if (!StoreWidgetUI)
	{
		StoreWidgetUI = CreateWidget<UTwoMinWidget_StoreUI>(GetWorld(), StoreWidgetClass);	
	}
}

void UPlayerUIComponent::OpenStoreWidget(ATwoMinPlayerCharacter* PlayerCharacter, AInteractionActor_NPC* NPC,
                                         const bool bIsOpenStoreWidget)
{
	if (!StoreWidgetClass) return;
	
	if (!PlayerCharacter) return;
	
	float TimeDilation;
	if (bIsOpenStoreWidget)
	{
		if (!NPC) return;
		
		TimeDilation = 0;
		StoreWidgetUI->AddToViewport();
		StoreWidgetUI->InitStoreUI(NPC, PlayerCharacter);
	}
	else
	{
		TimeDilation = 1.f;
		StoreWidgetUI->RemoveFromParent();
		PlayerCharacter->OnIgnoreInputProcess(false);
		NPC->ResetInteractionProcess();
	}
	
	bIsStoreWidgetOpen = bIsOpenStoreWidget;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

void UPlayerUIComponent::InitEndGameUI()
{
	if (!EndGameWidgetClass)
	{
		return;
	}
	
	if (!EndGameWidgetUI)
	{
		EndGameWidgetUI = CreateWidget<UTwoMinWidget_EndGameUI>(GetWorld(), EndGameWidgetClass);	
	}
}

void UPlayerUIComponent::OpenEndGameWidget(const ATwoMinPlayerCharacter* PlayerCharacter,
	const bool bIsOpenEndGameWidget)
{
	if (!EndGameWidgetClass) return;
	
	if (!PlayerCharacter) return;
	
	float TimeDilation;
	if (bIsOpenEndGameWidget)
	{
		TimeDilation = 0;
		EndGameWidgetUI->AddToViewport();
		EndGameWidgetUI->InitEndGameUI();
	}
	else
	{
		TimeDilation = 1.f;
		EndGameWidgetUI->ResetEndGameUI();
		EndGameWidgetUI->RemoveFromParent();
	}
	
	bIsEndGameWidgetOpen = bIsOpenEndGameWidget;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}