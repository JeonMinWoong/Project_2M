// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMode/TwoMinWidget_ScreenFadeInOut.h"

#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/SoundManager.h"

void UTwoMinWidget_ScreenFadeInOut::StartFadeOut(const FName StagePath, const FName StageName)
{
	if (IsAnimationPlaying(OnFadeOutAnim))
	{
		StopAnimation(OnFadeOutAnim);	
	}
	
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	GI->LockPlayerInput(true);
	
	NextStagePath = StagePath;
	NextStageName = StageName;
	PlayAnimation(OnFadeOutAnim);
}

void UTwoMinWidget_ScreenFadeInOut::StartFadeIn()
{
	if (IsAnimationPlaying(OnFadeInAnim))
	{
		StopAnimation(OnFadeInAnim);
	}
	
	PlayAnimation(OnFadeInAnim);
}

bool UTwoMinWidget_ScreenFadeInOut::IsPlayingFadeOut() const
{
	return IsAnimationPlaying(OnFadeOutAnim);
}

void UTwoMinWidget_ScreenFadeInOut::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (OnFadeOutAnim)
	{
		CompleteFadeOutAnimEvent.BindDynamic(this, &UTwoMinWidget_ScreenFadeInOut::CompleteFadeOutAnim);
		BindToAnimationFinished(OnFadeOutAnim, CompleteFadeOutAnimEvent);
	}
	
	if (OnFadeInAnim)
	{
		StartFadeInAnimEvent.BindDynamic(this, &UTwoMinWidget_ScreenFadeInOut::StartFadeInAnim);
		BindToAnimationStarted(OnFadeInAnim, StartFadeInAnimEvent);
		
		CompleteFadeInAnimEvent.BindDynamic(this, &UTwoMinWidget_ScreenFadeInOut::CompleteFadeInAnim);
		BindToAnimationFinished(OnFadeInAnim, CompleteFadeInAnimEvent);
	}
}

void UTwoMinWidget_ScreenFadeInOut::CompleteFadeOutAnim()
{
	if (NextStageName.IsNone()) return;

	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	GI->bIsStageMoving = true;
	
	const FName CachedNextStagePath = NextStagePath;
	NextStagePath = NAME_None;
	const FName CachedNextStageName = NextStageName;
	NextStageName = NAME_None;
	
	GI->ShowLoadingScreen(CachedNextStagePath, CachedNextStageName);
	GI->SoundManager->StopBGMSound();
}

void UTwoMinWidget_ScreenFadeInOut::StartFadeInAnim()
{
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	GI->LockPlayerInput(true);
}

void UTwoMinWidget_ScreenFadeInOut::CompleteFadeInAnim()
{
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	GI->LockPlayerInput(false);
	
	RemoveFromParent();
}
