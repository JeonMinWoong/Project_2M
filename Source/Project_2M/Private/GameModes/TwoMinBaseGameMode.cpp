// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TwoMinBaseGameMode.h"

#include "Blueprint/UserWidget.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Widgets/GameMode/TwoMinWidget_ScreenFadeInOut.h"

void ATwoMinBaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	if (!GI->bIsStageMoving) return;
	
	GI->bIsStageMoving = false;
	
	if (!FadeInOutWidgetClass) return;
	
	if (!FadeInOutWidget)
	{
		FadeInOutWidget = CreateWidget<UTwoMinWidget_ScreenFadeInOut>(GetWorld(), FadeInOutWidgetClass);	
	}
	
	FadeInOutWidget->AddToViewport();
	FadeInOutWidget->StartFadeIn();
}

void ATwoMinBaseGameMode::OpenStageProcess(const FName StageName)
{
	if (IsOpeningStage()) return;

	if (!FadeInOutWidgetClass) return;
	
	if (!FadeInOutWidget)
	{
		FadeInOutWidget = CreateWidget<UTwoMinWidget_ScreenFadeInOut>(GetWorld(), FadeInOutWidgetClass);	
	}
	
	FadeInOutWidget->AddToViewport();
	FadeInOutWidget->StartFadeOut(StageName);
}

bool ATwoMinBaseGameMode::IsOpeningStage() const
{
	return FadeInOutWidget && FadeInOutWidget->IsPlayingFadeOut();
}
