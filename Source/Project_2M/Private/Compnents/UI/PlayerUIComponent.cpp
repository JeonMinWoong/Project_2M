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

void UPlayerUIComponent::OpenMapSelectWidget(ATwoMinPlayerCharacter* PlayerCharacter, const bool bIsOpenMapSelectWidget)
{
	if (!MapSelectWidgetClass) return;
	
	if (!PlayerCharacter) return;
	
	float TimeDilation;
	if (bIsOpenMapSelectWidget)
	{
		PlayerCharacter->OnIgnoreInputProcess(true);
		TimeDilation = 0.0001f;
		MapSelectUI->AddToViewport();
		MapSelectUI->InitStageButtons();
		MapSelectUI->OnFocusSlot();
	}
	else
	{
		PlayerCharacter->OnIgnoreInputProcess(false);
		TimeDilation = 1.f;
		MapSelectUI->RemoveFromParent();
	}
	
	PlayerCharacter->OnUIInputApply(bIsOpenMapSelectWidget);
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
	
	PlayerCharacter->OnUIInputApply(bIsOpenStoreWidget);
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

void UPlayerUIComponent::OpenEndGameWidget(ATwoMinPlayerCharacter* PlayerCharacter,
	const bool bIsOpenEndGameWidget)
{
	if (!EndGameWidgetClass) return;
	
	if (!PlayerCharacter) return;
	
	float TimeDilation;
	if (bIsOpenEndGameWidget)
	{
		PlayerCharacter->OnIgnoreInputProcess(true);
		TimeDilation = 0.0001f;
		EndGameWidgetUI->AddToViewport();
		EndGameWidgetUI->InitEndGameUI();
	}
	else
	{
		PlayerCharacter->OnIgnoreInputProcess(false);
		TimeDilation = 1.f;
		EndGameWidgetUI->ResetEndGameUI();
		EndGameWidgetUI->RemoveFromParent();
	}
	
	PlayerCharacter->OnUIInputApply(bIsOpenEndGameWidget);
	bIsEndGameWidgetOpen = bIsOpenEndGameWidget;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}