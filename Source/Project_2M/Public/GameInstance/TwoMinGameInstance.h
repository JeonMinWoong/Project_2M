// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TwoMinConstant.h"
#include "Engine/GameInstance.h"
#include "TwoMinGameInstance.generated.h"

class UTwoMinWidget_LoadingUI;
enum class EUISoundType : uint8;
class USoundManager;
class UWorldStageManager;
class UItemDataManager;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void OnStart() override;
	
	void LockPlayerInput(bool bLock) const;
	void PlayUISound(const EUISoundType NewUISoundType) const;
	
	void ShowLoadingScreen(const FName& NextStagePath, const FName& NextStageName);
	void HideLoadingScreen();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	UItemDataManager* ItemDataManager;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	UWorldStageManager* StageManager;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	USoundManager* SoundManager;
	
	UPROPERTY(EditDefaultsOnly, Category="Loading")
	TSubclassOf<UTwoMinWidget_LoadingUI> LoadingWidgetClass;

	UPROPERTY()
	TObjectPtr<UTwoMinWidget_LoadingUI> LoadingWidget;
	
	UPROPERTY()
	FName PendingLevelName;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsStageMoving = false;
	
	FTimerHandle ApplyInitSettingsTimerHandle;
	
private:
	void ApplyInitSettings();

	void OnLevelLoaded(const FName& PackageName, UPackage* Package, EAsyncLoadingResult::Type Result);
	
	UPROPERTY()
	int32 InitGeneralValue = 1;
	
	UPROPERTY()
	float InitFrameRate = 144.f;
	
	UPROPERTY()
	float InitResolutionScaleValue = 100.f;
	
	UPROPERTY()
	FIntPoint InitScreenSize = TwoMinConstant::SupportedResolutions[3]; 
	
	UPROPERTY()
	bool bIsInitVSync = false;
	
	UPROPERTY()
	bool bIsInitScreenFullMode = true;
	
	UPROPERTY()
	float MinLoadingTime = 1.f;
	
	UPROPERTY()
	float CurLoadingTime = 0;
	
	FTimerHandle OpenLevelTimerHandle;
};
