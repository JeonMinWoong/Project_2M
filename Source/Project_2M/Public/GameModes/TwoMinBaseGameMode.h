// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwoMinBaseGameMode.generated.h"

struct FSaveGameData;
class ASpawnMonsterPointGroup;
class UTwoMinWidget_DefeatStageUI;
class UTwoMinWidgetBase;
class UTwoMinWidget_ClearStageUI;
class UTwoMinWidget_ScreenFadeInOut;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void LoadSaveDataProcess(FSaveGameData& LoadSaveGameData);
	void AfterBeginPlay();
	
	void OpenStageProcess(const FName StageName, bool bIsSaveData = true);
	void CreateNewSaveGameData(FSaveGameData& NewSaveGameData) const;
	
	bool IsOpeningStage() const;
	
	void ShowClearStageUI();
	void ShowDefeatStageUI();
	void LockPlayerInput(bool bLock, UTwoMinWidgetBase* InWidget) const;

private:
	UPROPERTY(EditDefaultsOnly, Category="GameModeWidget|FadeInOut")
	TSubclassOf<UTwoMinWidget_ScreenFadeInOut> FadeInOutWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_ScreenFadeInOut* FadeInOutWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="GameModeWidget|ClearStage")
	TSubclassOf<UTwoMinWidget_ClearStageUI> ClearStageWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_ClearStageUI* ClearStageWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="GameModeWidget|DefeatStage")
	TSubclassOf<UTwoMinWidget_DefeatStageUI> DefeatStageWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_DefeatStageUI* DefeatStageWidget;
	
	UPROPERTY()
	ASpawnMonsterPointGroup* SpawnMonsterPointGroup;
	
	UPROPERTY()
	bool bIsCompleteLoadSaveData = false;
	
public:
	FORCEINLINE ASpawnMonsterPointGroup* GetSpawnMonsterPointGroup() const { return SpawnMonsterPointGroup; }
	FORCEINLINE bool IsLoadData() const { return bIsCompleteLoadSaveData; }
	
};
