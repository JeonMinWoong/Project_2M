// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TwoMinBaseGameMode.h"

#include "Blueprint/UserWidget.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/TwoMinWidget_ClearStageUI.h"
#include "Widgets/TwoMinWidget_DefeatStageUI.h"
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
	
	FadeInOutWidget->AddToViewport(1000);
	FadeInOutWidget->StartFadeOut(StageName);
}

bool ATwoMinBaseGameMode::IsOpeningStage() const
{
	return FadeInOutWidget && FadeInOutWidget->IsPlayingFadeOut();
}

void ATwoMinBaseGameMode::ShowClearStageUI()
{
	if (!ClearStageWidgetClass) return;
	
	if (!ClearStageWidget)
	{
		ClearStageWidget = CreateWidget<UTwoMinWidget_ClearStageUI>(GetWorld(), ClearStageWidgetClass);	
	}
	
	ClearStageWidget->AddToViewport();
	ClearStageWidget->SettingClearStageUI();
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);
}

void ATwoMinBaseGameMode::ShowDefeatStageUI()
{
	if (!DefeatStageWidgetClass) return;
	
	if (!DefeatStageWidget)
	{
		DefeatStageWidget = CreateWidget<UTwoMinWidget_DefeatStageUI>(GetWorld(), DefeatStageWidgetClass);	
	}
	
	DefeatStageWidget->AddToViewport();
	DefeatStageWidget->SettingDefeatStageUI();
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);
}

void ATwoMinBaseGameMode::LockPlayerInput(bool bLock, UTwoMinWidgetBase* InWidget) const
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	PC->SetIgnoreMoveInput(bLock);
	PC->SetIgnoreLookInput(bLock);
	if (bLock)
	{
		PC->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(InWidget->TakeWidget()));
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}
