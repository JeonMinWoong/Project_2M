// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_SettingUI.h"

#include "TwoMinConstant.h"
#include "TwoMinFunctionLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"
#include "GameModes/TwoMinTitleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidget_BaseButton.h"
#include "Widgets/TwoMinWidget_CheckBoxButton.h"
#include "Widgets/TwoMinWidget_CustomProgressbar.h"

void UTwoMinWidget_SettingUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	InitSettingLoadData();
	InitSettingUI();
	OnFocusSlot();
}

void UTwoMinWidget_SettingUI::InitSettingLoadData()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings)
	{
		ResolutionQuality = Settings->GetResolutionScaleNormalized();
		FrameRateLimit = Settings->GetFrameRateLimit();
		bIsFullScreen = Settings->GetFullscreenMode() == EWindowMode::Fullscreen;
		ScreenSize = Settings->GetScreenResolution();
		GraphicQuality = Settings->GetOverallScalabilityLevel();
		bIsVeSync = Settings->IsVSyncEnabled();
	}
	
	FSoundSaveData LoadSoundData;
	if (UTwoMinFunctionLibrary::TryLoadSoundData(LoadSoundData))
	{
		MasterVolume = LoadSoundData.MasterVolume;
		bIsMasterMute = LoadSoundData.bIsMasterMute;
		MusicVolume = LoadSoundData.MusicVolume;
		bIsMusicMute = LoadSoundData.bIsMusicMute;
		SFXVolume = LoadSoundData.SFXVolume;
		bIsSFXMute = LoadSoundData.bIsSFXMute;
	}
	
	ScreenModeIndex = bIsFullScreen ? 0 : 1;
	ScreenSizeIndex = GetScreenSizeIndex(ScreenSize);
	GraphicQualityIndex = GetGraphicQualityIndex(GraphicQuality);
	VerticalSynchronizationIndex = bIsVeSync ? 1 : 0;
	MasterVolumeIndex = bIsMasterMute ? 1 : 0;
	MusicVolumeIndex = bIsMusicMute ? 1 : 0;
	SFXVolumeIndex = bIsSFXMute ? 1 : 0;
	ApplySettingIndex = 0;
	CurCategoryIndex = 0;
}

void UTwoMinWidget_SettingUI::InitSettingUI()
{
	if (ScreenModeCanvas)
	{
		TArray<UWidget*> ScreenModeWidgetGroup = ScreenModeCanvas->GetAllChildren();
		if (ScreenModeWidgetGroup.IsEmpty()) return;

		for (UWidget* ScreenModeWidget : ScreenModeWidgetGroup)
		{
			if (USizeBox* SizeBox = Cast<USizeBox>(ScreenModeWidget))
			{
				UWidget* ChildWidget = SizeBox->GetChildAt(0);
				if (!ChildWidget) continue;
				
				if (UTwoMinWidget_BaseButton* CastButton = Cast<UTwoMinWidget_BaseButton>(ChildWidget))
				{
					ScreenModeButtons.Add(CastButton);
					CastButton->SetLocked(true);
				}
			}
		}	
		
		ScreenModeButtons[ScreenModeIndex]->SetLocked(false);
		ScreenModeButtons[ScreenModeIndex]->SetKeepSelect(true);
	}
	
	if (ScreenSizeCanvas)
	{
		TArray<UWidget*> ScreenSizeGroup = ScreenSizeCanvas->GetAllChildren();
		if (ScreenSizeGroup.IsEmpty()) return;

		for (UWidget* ScreenSizeWidget : ScreenSizeGroup)
		{
			if (USizeBox* SizeBox = Cast<USizeBox>(ScreenSizeWidget))
			{
				UWidget* ChildWidget = SizeBox->GetChildAt(0);
				if (!ChildWidget) continue;
				
				if (UTextBlock* CastTextBlock = Cast<UTextBlock>(ScreenSizeWidget))
				{
					ScreenSizeTextBlocks.Add(CastTextBlock);
					continue;
				}
			
				if (UTwoMinWidget_BaseButton* CastButton = Cast<UTwoMinWidget_BaseButton>(ScreenSizeWidget))
				{
					ScreenSizeButton = CastButton;
				}
			}
		}
		
		SetScreenSizeText();
		ScreenSizeButton->SetKeepSelect(true);
		if (ScreenSizeDecreaseText)
		{
			ScreenSizeDecreaseText->SetVisibility(ESlateVisibility::Hidden);
		}
		
		if (ScreenSizeIncreaseText)
		{
			ScreenSizeIncreaseText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	if (GraphicQualityCanvas)
	{
		TArray<UWidget*> GraphicQualityWidgetGroup = GraphicQualityCanvas->GetAllChildren();
		if (GraphicQualityWidgetGroup.IsEmpty()) return;

		for (UWidget* GraphicQualityWidget : GraphicQualityWidgetGroup)
		{
			if (USizeBox* SizeBox = Cast<USizeBox>(GraphicQualityWidget))
			{
				UWidget* ChildWidget = SizeBox->GetChildAt(0);
				if (!ChildWidget) continue;
				
				if (UTwoMinWidget_BaseButton* CastButton = Cast<UTwoMinWidget_BaseButton>(ChildWidget))
				{
					GraphicQualityButtons.Add(CastButton);
					CastButton->SetLocked(true);
				}
			}
		}	
		
		GraphicQualityButtons[GraphicQualityIndex]->SetLocked(false);
		GraphicQualityButtons[GraphicQualityIndex]->SetKeepSelect(true);
	}
	
	if (VerticalSynchronizationCheckBox)
	{
		VerticalSynchronizationCheckBox->SetCheckBox(VerticalSynchronizationIndex != 0);
	}
	
	if (MasterVolumeProgressbar)
	{
		MasterVolumeProgressbar->InitProgressbar(MasterVolume, bIsMasterMute);
	}
	
	if (MusicVolumeProgressbar)
	{
		MusicVolumeProgressbar->InitProgressbar(MusicVolume, bIsMusicMute);
	}
	
	if (SFXVolumeProgressbar)
	{
		SFXVolumeProgressbar->InitProgressbar(SFXVolume, bIsSFXMute);
	}
	
	if (ConfirmCanvas)
	{
		TArray<UWidget*> ConfirmWidgetGroup = ConfirmCanvas->GetAllChildren();
		if (ConfirmWidgetGroup.IsEmpty()) return;

		for (UWidget* ConfirmWidget : ConfirmWidgetGroup)
		{
			if (USizeBox* SizeBox = Cast<USizeBox>(ConfirmWidget))
			{
				UWidget* ChildWidget = SizeBox->GetChildAt(0);
				if (!ChildWidget) continue;
				
				if (UTwoMinWidget_BaseButton* CastButton = Cast<UTwoMinWidget_BaseButton>(ChildWidget))
				{
					ConfirmButtons.Add(CastButton);
				}
			}
		}	
	}
}

int32 UTwoMinWidget_SettingUI::GetGraphicQualityIndex(int32 InGraphicQuality)
{
	switch (InGraphicQuality)
	{
	case 0:
		return 0;
	case 1:
		return 1;
	case 2:
		return 2;
	default:
		return 1;
	}
}

int32 UTwoMinWidget_SettingUI::GetScreenSizeIndex(FIntPoint InScreenSize)
{
	int32 FoundIndex = TwoMinConstant::SupportedResolutions.IndexOfByKey(InScreenSize);
	return FoundIndex != INDEX_NONE ? FoundIndex : 1;
}

void UTwoMinWidget_SettingUI::SetScreenSizeText() const
{
	FString ScreenSizeString = FString::Printf(TEXT("%d x %d"), ScreenSize.X, ScreenSize.Y);
	ScreenSizeButton->TextBox->SetText(FText::FromString(ScreenSizeString));
	
	const bool bIsUsingGamePad = UTwoMinFunctionLibrary::IsUsingGamePad(GetWorld(), GetOwningPlayer()->GetPlatformUserId());
	const FString DecreaseKeyText = bIsUsingGamePad ? TEXT("◀ LB") : TEXT("◀ Q");
	const FString IncreaseKeyText = bIsUsingGamePad ? TEXT("RB ▶") : TEXT("E ▶");
	
	if (ScreenSizeDecreaseText->GetText().ToString() == DecreaseKeyText) return;
	if (ScreenSizeIncreaseText->GetText().ToString() == IncreaseKeyText) return;
	
	ScreenSizeDecreaseText->SetText(FText::FromString(DecreaseKeyText));
	ScreenSizeIncreaseText->SetText(FText::FromString(IncreaseKeyText));
	
}

FReply UTwoMinWidget_SettingUI::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InKey = InKeyEvent.GetKey();
	
	if (InKey == EKeys::Enter || InKey == EKeys::F || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
		if (CurCategoryIndex == 4)
		{
			if (MasterVolumeIndex == 1)
			{
				bIsMasterMute = !bIsMasterMute;
				MasterVolumeProgressbar->SetCheckMuteButton(bIsMasterMute);
				MasterVolumeProgressbar->SetPercent(bIsMasterMute ? 0 : MasterVolume);
				TempApplySetting(EApplySettingType::SoundVolume);
				PlayUISound(EUISoundType::Focus_Select);
				
				return FReply::Handled();	
			}
		}
		else if (CurCategoryIndex == 5)
		{
			if (MusicVolumeIndex == 1)
			{
				bIsMusicMute = !bIsMusicMute;
				MusicVolumeProgressbar->SetCheckMuteButton(bIsMusicMute);
				MusicVolumeProgressbar->SetPercent(bIsMusicMute ? 0 : MusicVolume);
				TempApplySetting(EApplySettingType::SoundVolume);
				PlayUISound(EUISoundType::Focus_Select);
				
				return FReply::Handled();	
			}
		}
		else if (CurCategoryIndex == 6)
		{
			if (SFXVolumeIndex == 1)
			{
				bIsSFXMute = !bIsSFXMute;
				SFXVolumeProgressbar->SetCheckMuteButton(bIsSFXMute);
				SFXVolumeProgressbar->SetPercent(bIsSFXMute ? 0 : SFXVolume);
				TempApplySetting(EApplySettingType::SoundVolume);
				PlayUISound(EUISoundType::Focus_Select);
				
				return FReply::Handled();	
			}
		}
		else if (CurCategoryIndex == 7)
		{
			if (ApplySettingIndex == 0)
			{
				ApplySettingData();
				PlayUISound(EUISoundType::Focus_Select);
				CloseSettingWindow();
				
				return FReply::Handled();
			}
			else
			{
				ResetSettingData();
				PlayUISound(EUISoundType::Focus_Select);
				CloseSettingWindow();
				
				return FReply::Handled();
			}
		}
		
		return FReply::Unhandled();
	}
	else if (InKey == EKeys::Escape || InKey == EKeys::Gamepad_FaceButton_Right)
	{
		// 취소 창 닫기 및 설정 리셋
		ResetSettingData();
		PlayUISound(EUISoundType::Cancel);
		CloseSettingWindow();
		
		return FReply::Handled();
	}
	else if (InKey == EKeys::Q || InKey == EKeys::Gamepad_LeftShoulder)
	{
		if (CurCategoryIndex == 1)
		{
			if (ScreenSizeIndex <= 0) return FReply::Unhandled();
			
			ScreenSizeIndex--;
			ScreenSize = TwoMinConstant::SupportedResolutions[ScreenSizeIndex];
			TempApplySetting(EApplySettingType::ScreenSize);
			SetScreenSizeText();
			PlayUISound(EUISoundType::Focus_Move);
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 4)
		{
			if (MasterVolumeIndex != 0) return FReply::Unhandled();
			if (MasterVolume <= 0) return FReply::Unhandled();
			
			MasterVolume += DecreaseValue;
			MasterVolume = FMath::Clamp(MasterVolume, 0, 1);
			MasterVolumeProgressbar->SetPercent(bIsMasterMute ? 0 : MasterVolume);
			TempApplySetting(EApplySettingType::SoundVolume);
			PlayUISound(EUISoundType::Focus_Move);
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 5)
		{
			if (MusicVolumeIndex != 0) return FReply::Unhandled();
			if (MusicVolume <= 0) return FReply::Unhandled();
			
			MusicVolume += DecreaseValue;
			MusicVolume = FMath::Clamp(MusicVolume, 0, 1);
			MusicVolumeProgressbar->SetPercent(bIsMusicMute ? 0 : MusicVolume);
			TempApplySetting(EApplySettingType::SoundVolume);
			PlayUISound(EUISoundType::Focus_Move);
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 6)
		{
			if (SFXVolumeIndex != 0) return FReply::Unhandled();
			if (SFXVolume <= 0) return FReply::Unhandled();
			
			SFXVolume += DecreaseValue;
			SFXVolume = FMath::Clamp(SFXVolume, 0, 1);
			SFXVolumeProgressbar->SetPercent(bIsSFXMute ? 0 : SFXVolume);
			TempApplySetting(EApplySettingType::SoundVolume);
			PlayUISound(EUISoundType::Focus_Move);
			
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	else if (InKey == EKeys::E || InKey == EKeys::Gamepad_RightShoulder)
	{
		if (CurCategoryIndex == 1)
		{
			if (ScreenSizeIndex >= TwoMinConstant::SupportedResolutions.Num() - 1) return FReply::Unhandled();
			
			ScreenSizeIndex++;
			ScreenSize = TwoMinConstant::SupportedResolutions[ScreenSizeIndex];
			TempApplySetting(EApplySettingType::ScreenSize);
			SetScreenSizeText();
			PlayUISound(EUISoundType::Focus_Move);
			
			return FReply::Handled();
		}
		if (CurCategoryIndex == 4)
		{
			if (MasterVolumeIndex != 0) return FReply::Unhandled();
			if (MasterVolume >= 1) return FReply::Unhandled();
			
			MasterVolume += IncreaseValue;
			MasterVolume = FMath::Clamp(MasterVolume, 0, 1);
			MasterVolumeProgressbar->SetPercent(bIsMasterMute ? 0 : MasterVolume);
			TempApplySetting(EApplySettingType::SoundVolume);
			PlayUISound(EUISoundType::Focus_Move);
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 5)
		{
			if (MusicVolumeIndex != 0) return FReply::Unhandled();
			if (MusicVolume >= 1) return FReply::Unhandled();
			
			MusicVolume += IncreaseValue;
			MusicVolume = FMath::Clamp(MusicVolume, 0, 1);
			MusicVolumeProgressbar->SetPercent(bIsMusicMute ? 0 : MusicVolume);
			TempApplySetting(EApplySettingType::SoundVolume);
			PlayUISound(EUISoundType::Focus_Move);
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 6)
		{
			if (SFXVolumeIndex != 0) return FReply::Unhandled();
			if (SFXVolume >= 1) return FReply::Unhandled();
			
			SFXVolume += IncreaseValue;
			SFXVolume = FMath::Clamp(SFXVolume, 0, 1);
			SFXVolumeProgressbar->SetPercent(bIsSFXMute ? 0 : SFXVolume);
			TempApplySetting(EApplySettingType::SoundVolume);
			PlayUISound(EUISoundType::Focus_Move);
			
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	
	else if (InKey == EKeys::Right || InKey == EKeys::D || InKey == EKeys::Gamepad_LeftStick_Right)
	{
		if (CurCategoryIndex == 0)
		{
			if (ScreenModeIndex >= 1) return FReply::Unhandled();
			
			ScreenModeIndex = 1;
			bIsFullScreen = false;
			
			TempApplySetting(EApplySettingType::ScreenMode);
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 1)
		{
			if (ScreenSizeIndex >= TwoMinConstant::SupportedResolutions.Num() - 1) return FReply::Unhandled();

			ScreenSizeIndex++;
			ScreenSize = TwoMinConstant::SupportedResolutions[ScreenSizeIndex];
			TempApplySetting(EApplySettingType::ScreenSize);
			PlayUISound(EUISoundType::Focus_Move);
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 2)
		{
			if (GraphicQualityIndex >= 2) return FReply::Unhandled();
			
			GraphicQualityIndex++;
			GraphicQuality++;
			TempApplySetting(EApplySettingType::GraphicQuality);
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 3)
		{
			if (VerticalSynchronizationIndex >= 1) return FReply::Unhandled();
			
			VerticalSynchronizationIndex = 1;
			bIsVeSync = true;
			TempApplySetting(EApplySettingType::VSync);
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 4)
		{
			if (MasterVolumeIndex >= 1) return FReply::Unhandled();
			
			MasterVolumeIndex = 1;
			 
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 5)
		{
			if (MusicVolumeIndex >= 1) return FReply::Unhandled();
			
			MusicVolumeIndex = 1;
			
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 6)
		{
			if (SFXVolumeIndex >= 1) return FReply::Unhandled();
			
			SFXVolumeIndex = 1;
			
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 7)
		{
			if (ApplySettingIndex >= 1) return FReply::Unhandled();
			
			ApplySettingIndex = 1;
			
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	else if (InKey == EKeys::Left || InKey == EKeys::A || InKey == EKeys::Gamepad_LeftStick_Left)
	{
		if (CurCategoryIndex == 0)
		{
			if (ScreenModeIndex == 0) return FReply::Unhandled();
			
			ScreenModeIndex = 0;
			bIsFullScreen = true;
			
			TempApplySetting(EApplySettingType::ScreenMode);
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 1)
		{
			if (ScreenSizeIndex <= 0) return FReply::Unhandled();

			ScreenSizeIndex--;
			ScreenSize = TwoMinConstant::SupportedResolutions[ScreenSizeIndex];
			TempApplySetting(EApplySettingType::ScreenSize);
			PlayUISound(EUISoundType::Focus_Move);
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 2)
		{
			if (GraphicQualityIndex == 0) return FReply::Unhandled();
			
			GraphicQualityIndex--;
			GraphicQuality--;
			TempApplySetting(EApplySettingType::GraphicQuality);
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 3)
		{
			if (VerticalSynchronizationIndex == 0) return FReply::Unhandled();
			
			VerticalSynchronizationIndex = 0;
			bIsVeSync = false;
			TempApplySetting(EApplySettingType::VSync);
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 4)
		{
			if (MasterVolumeIndex <= 0) return FReply::Unhandled();
			
			MasterVolumeIndex = 0;
			
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 5)
		{
			if (MusicVolumeIndex <= 0) return FReply::Unhandled();
			
			MusicVolumeIndex = 0;
			
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 6)
		{
			if (SFXVolumeIndex <= 0) return FReply::Unhandled();
			
			SFXVolumeIndex = 0;
			
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		else if (CurCategoryIndex == 7)
		{
			if (ApplySettingIndex <= 0) return FReply::Unhandled();
			
			ApplySettingIndex = 0;
			
			PlayUISound(EUISoundType::Focus_Move);
			OnFocusSlot();
			
			return FReply::Handled();
		}
		
		return FReply::Unhandled();
	}
	else if (InKey == EKeys::Up || InKey == EKeys::W || InKey == EKeys::Gamepad_LeftStick_Up)
	{
		int32 NextCategoryIndex = CurCategoryIndex - 1;
		if (NextCategoryIndex < MinCategoryIndex) return FReply::Unhandled();
		
		CurCategoryIndex = NextCategoryIndex;
		
		PlayUISound(EUISoundType::Focus_Move);
		OnFocusSlot();
		
		return FReply::Handled();
	}
	else if (InKey == EKeys::Down || InKey == EKeys::S || InKey == EKeys::Gamepad_LeftStick_Down)
	{
		int32 NextCategoryIndex = CurCategoryIndex + 1;
		if (NextCategoryIndex > MaxCategoryIndex) return FReply::Unhandled();
		
		CurCategoryIndex = NextCategoryIndex;
		
		PlayUISound(EUISoundType::Focus_Move);
		OnFocusSlot();
		
		return FReply::Handled();
	}
	
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

void UTwoMinWidget_SettingUI::ApplySettingData() const
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings)
	{
		Settings->SetFullscreenMode(bIsFullScreen ? EWindowMode::Fullscreen : EWindowMode::Windowed);
		Settings->SetScreenResolution(ScreenSize);
		Settings->SetFrameRateLimit(FrameRateLimit);
		Settings->SetResolutionScaleValueEx(ResolutionQuality);
		Settings->SetOverallScalabilityLevel(GraphicQuality);
		Settings->SetVSyncEnabled(bIsVeSync);
		
		// Settings->SetViewDistanceQuality(1);
		// Settings->SetAntiAliasingQuality(1);
		// Settings->SetPostProcessingQuality(1);
		// Settings->SetShadowQuality(1);
		// Settings->SetGlobalIlluminationQuality(1);
		// Settings->SetReflectionQuality(1);
		// Settings->SetTextureQuality(1);
		// Settings->SetVisualEffectQuality(1);
		// Settings->SetFoliageQuality(1);
		// Settings->SetShadingQuality(1);
		
		// ApplySettings 대신 직접 적용
		Scalability::FQualityLevels QualityLevels;
		int32 CurGraphicQuality = GraphicQuality;
		 
		QualityLevels.ResolutionQuality = ResolutionQuality;
		QualityLevels.ViewDistanceQuality = CurGraphicQuality;
		QualityLevels.AntiAliasingQuality = CurGraphicQuality;
		QualityLevels.PostProcessQuality = CurGraphicQuality;
		QualityLevels.ShadowQuality = CurGraphicQuality;
		QualityLevels.GlobalIlluminationQuality = CurGraphicQuality;
		QualityLevels.ReflectionQuality = CurGraphicQuality;
		QualityLevels.TextureQuality = CurGraphicQuality;
		QualityLevels.EffectsQuality = CurGraphicQuality;
		QualityLevels.FoliageQuality = CurGraphicQuality;
		QualityLevels.ShadingQuality = CurGraphicQuality;
		QualityLevels.LandscapeQuality = CurGraphicQuality;
		Scalability::SetQualityLevels(QualityLevels);
		Scalability::SaveState(GGameUserSettingsIni);
        
		Settings->ApplySettings(false);
	}
	
	FSoundSaveData NewSoundSaveData;
	NewSoundSaveData.MasterVolume = MasterVolume;
	NewSoundSaveData.bIsMasterMute = bIsMasterMute;
	NewSoundSaveData.MusicVolume = MusicVolume;
	NewSoundSaveData.bIsMusicMute = bIsMusicMute;
	NewSoundSaveData.SFXVolume = SFXVolume;
	NewSoundSaveData.bIsSFXMute = bIsSFXMute;
	
	UTwoMinFunctionLibrary::SaveSoundData(NewSoundSaveData);
	UTwoMinFunctionLibrary::ApplySoundVolume(this, NewSoundSaveData);
}

void UTwoMinWidget_SettingUI::ResetSettingData()
{
	InitSettingLoadData();
	ApplySettingData();
}

void UTwoMinWidget_SettingUI::OnFocusSlot()
{
	ScreenSizeDecreaseText->SetVisibility(CurCategoryIndex == 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	ScreenSizeIncreaseText->SetVisibility(CurCategoryIndex == 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
	if (CurCategoryIndex == 0)
	{
		for (int32 Index = 0; Index < ScreenModeButtons.Num(); ++Index)
		{
			if (Index == ScreenModeIndex)
			{
				ScreenModeButtons[ScreenModeIndex]->SetLocked(false);
				ScreenModeButtons[ScreenModeIndex]->SetKeepSelect(true);
				ScreenModeButtons[ScreenModeIndex]->SetFocus();
				continue;
			}	
			
			ScreenModeButtons[Index]->SetLocked(true);
		}
	}
	else if (CurCategoryIndex == 1)
	{
		ScreenSizeButton->SetLocked(false);
		ScreenSizeButton->SetKeepSelect(true);
		ScreenSizeButton->SetFocus();
	}
	else if (CurCategoryIndex == 2)
	{
		for (int32 Index = 0; Index < GraphicQualityButtons.Num(); ++Index)
		{
			if (Index == GraphicQualityIndex)
			{
				GraphicQualityButtons[GraphicQualityIndex]->SetLocked(false);
				GraphicQualityButtons[GraphicQualityIndex]->SetKeepSelect(true);
				GraphicQualityButtons[GraphicQualityIndex]->SetFocus();
				continue;
			}	
			
			GraphicQualityButtons[Index]->SetLocked(true);
		}
	}
	else if (CurCategoryIndex == 3)
	{
		VerticalSynchronizationCheckBox->SetCheckBox(bIsVeSync);
		VerticalSynchronizationCheckBox->SetLocked(false);
		VerticalSynchronizationCheckBox->SetFocus();
	}
	else if (CurCategoryIndex == 4)
	{
		if (MasterVolumeIndex == 0)
		{
			MasterVolumeProgressbar->SetEnableInputText(true);
			MasterVolumeProgressbar->SetFocus();	
		}
		else
		{
			MasterVolumeProgressbar->SetFocusMuteButton();
		}
	}
	else if (CurCategoryIndex == 5)
	{
		if (MusicVolumeIndex == 0)
		{
			MusicVolumeProgressbar->SetEnableInputText(true);
			MusicVolumeProgressbar->SetFocus();
		}
		else
		{
			MusicVolumeProgressbar->SetFocusMuteButton();
		}
	}
	else if (CurCategoryIndex == 6)
	{
		if (SFXVolumeIndex == 0)
		{
			SFXVolumeProgressbar->SetEnableInputText(true);
			SFXVolumeProgressbar->SetFocus();
		}
		else
		{
			SFXVolumeProgressbar->SetFocusMuteButton();
		}
	}
	else if (CurCategoryIndex == 7)
	{
		ConfirmButtons[ApplySettingIndex]->SetFocus();
	}
}

void UTwoMinWidget_SettingUI::CloseSettingWindow()
{
	ATwoMinTitleGameMode* TitleGM = Cast<ATwoMinTitleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!TitleGM) return;
	
	RemoveFromParent();
	TitleGM->ReFocusingTitleUI();
}

void UTwoMinWidget_SettingUI::TempApplySetting(EApplySettingType ApplySettingType) const
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	
	switch (ApplySettingType) {
	case EApplySettingType::ScreenMode:
		{
			Settings->SetFullscreenMode(bIsFullScreen ? EWindowMode::Fullscreen : EWindowMode::Windowed);
			Settings->ApplyResolutionSettings(false);
		}
		break;
	case EApplySettingType::ScreenSize:
		{
			Settings->SetScreenResolution(ScreenSize);
			Settings->ApplyResolutionSettings(false);
		}
		break;
	case EApplySettingType::GraphicQuality:
		{
			Settings->SetOverallScalabilityLevel(GraphicQuality);
			Settings->ApplyNonResolutionSettings();
		}
		break;
	case EApplySettingType::VSync:
		{
			Settings->SetVSyncEnabled(bIsVeSync);
			Settings->ApplyResolutionSettings(false);
		}
		break;
	case EApplySettingType::SoundVolume:
		{
			FSoundSaveData TempSoundSaveData;
			TempSoundSaveData.MasterVolume = MasterVolume;
			TempSoundSaveData.bIsMasterMute = bIsMasterMute;
			TempSoundSaveData.MusicVolume = MusicVolume;
			TempSoundSaveData.bIsMusicMute = bIsMusicMute;
			TempSoundSaveData.SFXVolume = SFXVolume;
			TempSoundSaveData.bIsSFXMute = bIsSFXMute;
			
			UTwoMinFunctionLibrary::ApplySoundVolume(this, TempSoundSaveData);
		}
		break;
	}
}

