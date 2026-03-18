// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SoundManager.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


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
