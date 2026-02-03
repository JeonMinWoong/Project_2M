// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwoMinBaseGameMode.generated.h"

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

private:
	UPROPERTY(EditDefaultsOnly, Category="GameModeWidget|FadeInOut")
	TSubclassOf<UTwoMinWidget_ScreenFadeInOut> FadeInOutWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_ScreenFadeInOut* FadeInOutWidget;
};
