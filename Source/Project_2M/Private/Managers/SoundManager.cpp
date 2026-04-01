// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SoundManager.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"


void USoundManager::PlayBGMSound(EBGMSoundType NewBGMSoundType)
{
	if (CurBGMSoundType == NewBGMSoundType) return;
	
	StopBGMSound();
	
	if (NewBGMSoundType == EBGMSoundType::None) return;
	if (BGNGroups.Contains(NewBGMSoundType) == false) return;

	CurBGMSoundType = NewBGMSoundType;
	BGMAudioComponent = UGameplayStatics::SpawnSound2D(this, BGNGroups[NewBGMSoundType], 
		1.0f, 1.0f, 0.0f, nullptr, true);
}

void USoundManager::StopBGMSound()
{
	if (!BGMAudioComponent) return;
	if (BGMAudioComponent->IsPlaying() == false) return;

	BGMAudioComponent->Stop();
	BGMAudioComponent = nullptr;
	CurBGMSoundType = EBGMSoundType::None;
}

void USoundManager::ApplySoundVolume(const UObject* WorldContextObject, float MasterVolume, bool bMasterMute, 
	float MusicVolume, bool bMusicMute, float SFXVolume, bool bSFXMute)
{
	if (!WorldContextObject) return;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;
	if (!SoundMix) return;

	UGameplayStatics::ClearSoundMixModifiers(World);
	
	if (MasterSoundClass)
	{
		const float Volume = bMasterMute ? 0.f : MasterVolume;
		UGameplayStatics::SetSoundMixClassOverride(World, SoundMix, MasterSoundClass, Volume, 
			1.f, 0.f, true);
	}

	if (MusicSoundClass)
	{
		const float Volume = bMusicMute ? 0.f : MusicVolume;
		UGameplayStatics::SetSoundMixClassOverride(World, SoundMix, MusicSoundClass, Volume, 
			1.f, 0.f, true);
	}

	if (SFXSoundClass)
	{
		const float Volume = bSFXMute ? 0.f : SFXVolume;
		UGameplayStatics::SetSoundMixClassOverride(World, SoundMix, SFXSoundClass, Volume, 
			1.f, 0.f, true);
	}
	
	UGameplayStatics::PushSoundMixModifier(World, SoundMix);
}
