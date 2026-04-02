// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_SettingUI.generated.h"

class UTwoMinWidget_CustomProgressbar;
class UTwoMinWidget_CheckBoxButton;
class UTextBlock;
class UCheckBox;
class UTwoMinWidget_BaseButton;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_SettingUI : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	
private:
	void InitSettingLoadData();
	void InitSettingUI();
	
	int32 GetGraphicQualityIndex(int32 InGraphicQuality);
	int32 GetScreenSizeIndex(FIntPoint InScreenSize);
	
	void SetScreenSizeText() const;
	void ApplySettingData() const;
	void ResetSettingData();
	void OnFocusSlot();
	void CloseSettingWindow();
	
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* ScreenModeCanvas;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_BaseButton>> ScreenModeButtons;
	
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* ScreenSizeCanvas;
	
	UPROPERTY()
	TArray<TObjectPtr<UTextBlock>> ScreenSizeTextBlocks;
	
	UPROPERTY()
	TObjectPtr<UTwoMinWidget_BaseButton> ScreenSizeButton;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScreenSizeDecreaseText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScreenSizeIncreaseText;
	
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* GraphicQualityCanvas;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_BaseButton>> GraphicQualityButtons;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_CheckBoxButton* VerticalSynchronizationCheckBox;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_CustomProgressbar* MasterVolumeProgressbar;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_CustomProgressbar* MusicVolumeProgressbar;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_CustomProgressbar* SFXVolumeProgressbar;
	
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* ConfirmCanvas;
	
	UPROPERTY()
	TArray<TObjectPtr<UTwoMinWidget_BaseButton>> ConfirmButtons;
	
	UPROPERTY()
	int32 ScreenModeIndex = 0;
	
	UPROPERTY()
	int32 ScreenSizeIndex = 0;
	
	UPROPERTY()
	int32 GraphicQualityIndex = 0;
	
	UPROPERTY()
	int32 VerticalSynchronizationIndex = 0;
	
	UPROPERTY()
	int32 MasterVolumeIndex = 0;
	
	UPROPERTY()
	int32 MusicVolumeIndex = 0;
	
	UPROPERTY()
	int32 SFXVolumeIndex = 0;
	
	UPROPERTY()
	int32 ApplySettingIndex = 0;
	
	UPROPERTY()
	int32 MinCategoryIndex = 0;
	
	UPROPERTY()
	int32 MaxCategoryIndex = 7;
	
	UPROPERTY()
	int32 CurCategoryIndex = 0; 
	
	UPROPERTY()
	float ResolutionQuality = 100.f;
	
	UPROPERTY()
	float FrameRateLimit = 144.f;
	
	UPROPERTY()
	bool bIsFullScreen = false;
	
	UPROPERTY()
	FIntPoint ScreenSize;
	
	UPROPERTY()
	int32 GraphicQuality;
	
	UPROPERTY()
	bool bIsVerticalSynchronization = false;
	
	UPROPERTY()
	float MasterVolume = 1.0f;
	
	UPROPERTY()
	bool bIsMasterMute = false;
	
	UPROPERTY()
	float MusicVolume = 1.0f;
	
	UPROPERTY()
	bool bIsMusicMute = false;
	
	UPROPERTY()
	float SFXVolume = 1.0f;
	
	UPROPERTY()
	bool bIsSFXMute = false;
	
	UPROPERTY()
	float IncreaseValue = 0.01f;
	
	UPROPERTY()
	float DecreaseValue = -0.01f;
	
};
