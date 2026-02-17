// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwoMinBaseGameMode.generated.h"

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
	
	void OpenStageProcess(const FName StageName);
	bool IsOpeningStage() const;
	
	void ShowClearStageUI();

private:
	UPROPERTY(EditDefaultsOnly, Category="GameModeWidget|FadeInOut")
	TSubclassOf<UTwoMinWidget_ScreenFadeInOut> FadeInOutWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_ScreenFadeInOut* FadeInOutWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="GameModeWidget|ClearStage")
	TSubclassOf<UTwoMinWidget_ClearStageUI> ClearStageWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_ClearStageUI* ClearStageWidget;
};
