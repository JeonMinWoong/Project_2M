// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class PROJECT_2M_API USoundManager : public UObject
{
	GENERATED_BODY()
	
public:
	void PlayBGMSound(EBGMSoundType NewBGMSoundType);
	void StopBGMSound();
	void ApplySoundVolume(const UObject* WorldContextObject, float MasterVolume, bool bMasterMute, float MusicVolume, 
		bool bMusicMute, float SFXVolume, bool bSFXMute);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "SoundInfo|SoundMix")
	USoundMix* SoundMix;

	UPROPERTY(EditDefaultsOnly, Category = "SoundInfo|SoundClass")
	USoundClass* MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "SoundInfo|SoundClass")
	USoundClass* MusicSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "SoundInfo|SoundClass")
	USoundClass* SFXSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "SoundInfo|BGM")
	TMap<EBGMSoundType, USoundBase*> BGNGroups;
	
	UPROPERTY()
	UAudioComponent* BGMAudioComponent;
	
	UPROPERTY()
	EBGMSoundType CurBGMSoundType;
	
	UPROPERTY(EditDefaultsOnly, Category = "SoundInfo|UISound")
	TMap<EUISoundType, USoundBase*> UISoundGroups;
	
public:
	FORCEINLINE USoundBase* GetUISound(const EUISoundType NewUISoundType)
	{
		if (NewUISoundType == EUISoundType::None) return nullptr;
		if (UISoundGroups.Contains(NewUISoundType) == false) return nullptr;
		
		return UISoundGroups[NewUISoundType];
	};
};
