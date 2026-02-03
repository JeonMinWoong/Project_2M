// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/WidgetAnimation.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_ScreenFadeInOut.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_ScreenFadeInOut : public UTwoMinWidgetBase
{
	GENERATED_BODY()

public:
	void StartFadeOut(const FName StageName);
	void StartFadeIn();
	bool IsPlayingFadeOut() const;
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	void LockPlayerInput(const bool bLock);
	
	UFUNCTION()
	void CompleteFadeOutAnim();
	
	UFUNCTION()
	void StartFadeInAnim();
	
	UFUNCTION()
	void CompleteFadeInAnim();
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OnFadeOutAnim;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OnFadeInAnim;
	
	UPROPERTY()
	FWidgetAnimationDynamicEvent CompleteFadeOutAnimEvent;
	
	UPROPERTY()
	FWidgetAnimationDynamicEvent StartFadeInAnimEvent;
	
	UPROPERTY()
	FWidgetAnimationDynamicEvent CompleteFadeInAnimEvent;

	UPROPERTY()
	FName NextStageName;
	
public:
	FORCEINLINE float GetFadeOutDuration() const { return OnFadeOutAnim->GetEndTime(); }
};
