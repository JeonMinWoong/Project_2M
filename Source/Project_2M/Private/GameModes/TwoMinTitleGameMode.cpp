// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TwoMinTitleGameMode.h"

#include "Widgets/TwoMinWidget_TitleUI.h"
#include "Widgets/GameMode/TwoMinWidget_ScreenFadeInOut.h"

void ATwoMinTitleGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	ShowTitleWidget();
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

void ATwoMinTitleGameMode::OnEnterInGame(FName NextStageName)
{
	if (!FadeInOutWidgetClass) return;
	
	if (!FadeInOutWidget)
	{
		FadeInOutWidget = CreateWidget<UTwoMinWidget_ScreenFadeInOut>(GetWorld(), FadeInOutWidgetClass);	
	}
	
	FadeInOutWidget->AddToViewport(1000);
	FadeInOutWidget->StartFadeOut(NextStageName);
}
