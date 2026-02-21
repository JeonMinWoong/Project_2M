// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMode/TwoMinWidget_ScreenFadeInOut.h"

#include "GameInstance/TwoMinGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UTwoMinWidget_ScreenFadeInOut::StartFadeOut(const FName StageName)
{
	if (IsAnimationPlaying(OnFadeOutAnim))
	{
		StopAnimation(OnFadeOutAnim);	
	}
	
	LockPlayerInput(true);
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

void UTwoMinWidget_ScreenFadeInOut::LockPlayerInput(const bool bLock)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	PC->SetIgnoreMoveInput(bLock);
	PC->SetIgnoreLookInput(bLock);
	if (bLock)
	{
		PC->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(TakeWidget()));
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}


void UTwoMinWidget_ScreenFadeInOut::CompleteFadeOutAnim()
{
	if (NextStageName.IsNone()) return;

	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	GI->bIsStageMoving = true;
	const FName CachedNextStageName = NextStageName;
	NextStageName = NAME_None;
	
	UGameplayStatics::OpenLevel(GetWorld(), CachedNextStageName);
}

void UTwoMinWidget_ScreenFadeInOut::StartFadeInAnim()
{
	LockPlayerInput(true);
}

void UTwoMinWidget_ScreenFadeInOut::CompleteFadeInAnim()
{
	LockPlayerInput(false);
	RemoveFromParent();
}
