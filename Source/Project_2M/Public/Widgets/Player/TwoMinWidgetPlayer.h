// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinCharacterWidgetBase.h"
#include "TwoMinWidgetPlayer.generated.h"

class UTwoMinWidget_GoldInfo;
struct FItemPickUpEntry;
class UTwoMinWidget_ItemPickUpWindow;
class UTextBlock;
class UProgressBar;
class UPlayerUIComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidgetPlayer : public UTwoMinCharacterWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void InitPlayerUIComponent(UPlayerUIComponent* HeroUIComponent);
	
	virtual void SetCurrentHealthPercent(float Percent) override;

	UFUNCTION()
	void SetCurrentStaminaPercent(float Percent);

	UFUNCTION()
	void SetCurrentExperiencePercent(float Percent);

	UFUNCTION()
	void SetCurrentLevelValue(int32 InCurrentLevel);

	UFUNCTION()
	void SetPossiblePickUpItem(bool bIsPossiblePickUp);
	
	UFUNCTION()
	void SetItemPickUpWindow(int32 ShowAllItem, TArray<FItemPickUpEntry>& ItemList);

	UFUNCTION()
	void SetCurrentGoldValue(int32 NewGold, int32 GainGold);
	
private:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ExperienceBar;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelText;

	UPROPERTY(meta=(BindWidget))
	UTwoMinWidgetBase* PickUpItem;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_ItemPickUpWindow* ItemPickUpWindow;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_GoldInfo* GoldInfo;
};
