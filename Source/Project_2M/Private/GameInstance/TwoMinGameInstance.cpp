// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/TwoMinGameInstance.h"

#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/SoundManager.h"
#include "Managers/WorldStageManager.h"
#include "Widgets/TwoMinWidget_LoadingUI.h"

void UTwoMinGameInstance::Init()
{
	Super::Init();
	
	ApplyInitSettings();
	StageManager->InitStage();
}

void UTwoMinGameInstance::ApplyInitSettings()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (Settings)
	{
		Settings->SetOverallScalabilityLevel(GeneralValue);
		Settings->SetViewDistanceQuality(1);
		Settings->SetAntiAliasingQuality(1);
		Settings->SetPostProcessingQuality(1);
		Settings->SetShadowQuality(1);
		Settings->SetGlobalIlluminationQuality(1);
		Settings->SetReflectionQuality(1);
		Settings->SetTextureQuality(1);
		Settings->SetVisualEffectQuality(1);
		Settings->SetFoliageQuality(1);
		Settings->SetShadingQuality(1);
		Settings->SetFrameRateLimit(144.0f);
		Settings->SetResolutionScaleValueEx(100.0f);
        
		// ApplySettings 대신 직접 적용
		Scalability::FQualityLevels QualityLevels;
		QualityLevels.ResolutionQuality = 100.f;
		QualityLevels.ViewDistanceQuality = 1;
		QualityLevels.AntiAliasingQuality = 1;
		QualityLevels.PostProcessQuality = 1;
		QualityLevels.ShadowQuality = 1;
		QualityLevels.GlobalIlluminationQuality = 1;
		QualityLevels.ReflectionQuality = 1;
		QualityLevels.TextureQuality = 1;
		QualityLevels.EffectsQuality = 1;
		QualityLevels.FoliageQuality = 1;
		QualityLevels.ShadingQuality = 1;
		QualityLevels.LandscapeQuality = 1;
		Scalability::SetQualityLevels(QualityLevels);
		Scalability::SaveState(GGameUserSettingsIni);
        
		Settings->SaveSettings();
	}
}

void UTwoMinGameInstance::LockPlayerInput(const bool bLock) const
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	PC->SetIgnoreMoveInput(bLock);
	PC->SetIgnoreLookInput(bLock);
	if (bLock)
	{
		PC->SetInputMode(FInputModeUIOnly());
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void UTwoMinGameInstance::PlayUISound(const EUISoundType NewUISoundType) const
{
	USoundBase* UISound = SoundManager->GetUISound(NewUISoundType);
	if (!UISound) return;
	
	UGameplayStatics::PlaySound2D(this, UISound, 1.0f, 1.0f, 0.0f, 
		nullptr, nullptr, true);
}

void UTwoMinGameInstance::ShowLoadingScreen(const FName& NextStagePath, const FName& NextStageName)
{
	if (!LoadingWidgetClass || LoadingWidget) return;
	
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
	LoadingWidget = CreateWidget<UTwoMinWidget_LoadingUI>(this, LoadingWidgetClass);
	LoadingWidget->AddToViewport(ToZOrder(EWidgetZOrderType::Loading));
	
	PendingLevelName = NextStageName;
	CurLoadingTime = GetWorld()->GetUnpausedTimeSeconds();
	
	LoadPackageAsync(NextStagePath.ToString(), 
		FLoadPackageAsyncDelegate::CreateUObject(this, &UTwoMinGameInstance::OnLevelLoaded), 
		0, 
		PKG_ContainsMap);
}

void UTwoMinGameInstance::OnLevelLoaded(const FName& PackageName,
	UPackage* Package, EAsyncLoadingResult::Type Result)
{
	if (Result != EAsyncLoadingResult::Succeeded) return;

	const float Elapsed = GetWorld()->GetUnpausedTimeSeconds() - CurLoadingTime;
	const float Remaining = FMath::Max(0.f, MinLoadingTime - Elapsed);
	
	FName LevelName = PendingLevelName;
	GetWorld()->GetTimerManager().SetTimer(OpenLevelTimerHandle, [this, LevelName]()
	{
		UGameplayStatics::OpenLevel(this, LevelName);
	}, FMath::Max(Remaining, 0.05f), false);
}

void UTwoMinGameInstance::HideLoadingScreen()
{
	if (!LoadingWidget) return;
	
	LoadingWidget->RemoveFromParent();
	LoadingWidget = nullptr;
}

