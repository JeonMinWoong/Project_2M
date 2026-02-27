// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwoMinTitleGameMode.generated.h"

class UTwoMinWidget_ScreenFadeInOut;
class UTwoMinWidget_TitleUI;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinTitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void ShowTitleWidget();
	void OnEnterInGame(FName NextStageName);
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTwoMinWidget_TitleUI> TitleWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_TitleUI* TitleWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="GameModeWidget|FadeInOut")
	TSubclassOf<UTwoMinWidget_ScreenFadeInOut> FadeInOutWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_ScreenFadeInOut* FadeInOutWidget;
};
