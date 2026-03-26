// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TwoMinTitleGameMode.h"

#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/SoundManager.h"
#include "Widgets/TwoMinWidget_TitleUI.h"
#include "Widgets/GameMode/TwoMinWidget_ScreenFadeInOut.h"

void ATwoMinTitleGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	InitGISetting();
	ShowTitleWidget();
}

void ATwoMinTitleGameMode::InitGISetting() const
{
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	GI->bIsStageMoving = false;
	GI->HideLoadingScreen();
	GI->LockPlayerInput(false);
	
	GI->SoundManager->PlayBGMSound(EBGMSoundType::Title);
}

void ATwoMinTitleGameMode::ShowTitleWidget()
{
	if (!TitleWidgetClass) return;
	
	if (!TitleWidget)
	{
		TitleWidget = CreateWidget<UTwoMinWidget_TitleUI>(GetWorld(), TitleWidgetClass);	
	}
	
	TitleWidget->AddToViewport();
	TitleWidget->OnFocusSlot();
}

void ATwoMinTitleGameMode::OnEnterInGame(const FName NextStagePath, const FName NextStageName)
{
	if (!FadeInOutWidgetClass) return;
	
	if (!FadeInOutWidget)
	{
		FadeInOutWidget = CreateWidget<UTwoMinWidget_ScreenFadeInOut>(GetWorld(), FadeInOutWidgetClass);	
	}
	
	FadeInOutWidget->AddToViewport(ToZOrder(EWidgetZOrderType::FadeInOut));
	FadeInOutWidget->StartFadeOut(NextStagePath, NextStageName);
}
