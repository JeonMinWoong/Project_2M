// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/TwoMinGameInstance.h"

#include "Brushes/SlateNoResource.h"
#include "GameFramework/GameUserSettings.h"
#include "Styling/AppStyle.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/SoundManager.h"
#include "Managers/WorldStageManager.h"
#include "TwoMinFunctionLibrary.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidget_LoadingUI.h"

void UTwoMinGameInstance::OnStart()
{
	Super::OnStart();

	// Slate 기본 포커스 사각형(파란 테두리) 제거
	const ISlateStyle* AppStyle = FSlateStyleRegistry::FindSlateStyle(FAppStyle::GetAppStyleSetName());
	if (AppStyle)
	{
		FSlateStyleSet* MutableStyle = const_cast<FSlateStyleSet*>(static_cast<const FSlateStyleSet*>(AppStyle));
		MutableStyle->Set("FocusRectangle", new FSlateNoResource());
	}

	ApplyInitSettings();
	StageManager->InitStage();
}

void UTwoMinGameInstance::ApplyInitSettings()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		Settings->SetOverallScalabilityLevel(InitGeneralValue);
		Settings->SetViewDistanceQuality(InitGeneralValue);
		Settings->SetAntiAliasingQuality(InitGeneralValue);
		Settings->SetPostProcessingQuality(InitGeneralValue);
		Settings->SetShadowQuality(InitGeneralValue);
		Settings->SetGlobalIlluminationQuality(InitGeneralValue);
		Settings->SetReflectionQuality(InitGeneralValue);
		Settings->SetTextureQuality(InitGeneralValue);
		Settings->SetVisualEffectQuality(InitGeneralValue);
		Settings->SetFoliageQuality(InitGeneralValue);
		Settings->SetShadingQuality(InitGeneralValue);
		Settings->SetFrameRateLimit(InitFrameRate);
		Settings->SetResolutionScaleValueEx(InitResolutionScaleValue);
		Settings->SetScreenResolution(InitScreenSize);
		Settings->SetVSyncEnabled(bIsInitVSync);
		Settings->SetFullscreenMode(bIsInitScreenFullMode ? EWindowMode::Fullscreen : EWindowMode::Windowed);
        
		// ApplySettings 대신 직접 적용
		Scalability::FQualityLevels QualityLevels;
		QualityLevels.ResolutionQuality = InitResolutionScaleValue;
		QualityLevels.ViewDistanceQuality = InitGeneralValue;
		QualityLevels.AntiAliasingQuality = InitGeneralValue;
		QualityLevels.PostProcessQuality = InitGeneralValue;
		QualityLevels.ShadowQuality = InitGeneralValue;
		QualityLevels.GlobalIlluminationQuality = InitGeneralValue;
		QualityLevels.ReflectionQuality = InitGeneralValue;
		QualityLevels.TextureQuality = InitGeneralValue;
		QualityLevels.EffectsQuality = InitGeneralValue;
		QualityLevels.FoliageQuality = InitGeneralValue;
		QualityLevels.ShadingQuality = InitGeneralValue;
		QualityLevels.LandscapeQuality = InitGeneralValue;
		Scalability::SetQualityLevels(QualityLevels);
		Scalability::SaveState(GGameUserSettingsIni);
        
		Settings->SaveSettings();
	}

	FSoundSaveData LoadedSoundData;
	if (UTwoMinFunctionLibrary::TryLoadSoundData(LoadedSoundData))
	{
		UTwoMinFunctionLibrary::ApplySoundVolume(this, LoadedSoundData);
	}
	else
	{
		UTwoMinFunctionLibrary::ApplySoundVolume(this, FSoundSaveData());
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

