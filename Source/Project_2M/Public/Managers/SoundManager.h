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
	
private:
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
