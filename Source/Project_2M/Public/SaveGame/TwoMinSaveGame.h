// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinSaveGame.generated.h"

struct FSaveGameData;
struct FItemInstance;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	void SavePlayerGameData(const FSaveGameData& NewSaveGameData);
	FSaveGameData LoadPlayerGameData() const;
	bool IsExistSavePlayerGameData() const;
	
	void SaveSoundData(const FSoundSaveData& NewSoundSaveData);
	FSoundSaveData LoadSoundData() const;

private:
	UPROPERTY()
	TMap<FString, bool> WorldStageMap;
	
	UPROPERTY()
	int32 PlayerLevel;
	
	UPROPERTY()
	int32 PlayerCurrentExp;
	
	UPROPERTY()
	int32 PlayerCurrentGold;
	
	UPROPERTY()
	TArray<FItemInstance> PlayerCurrentItems;
	
	UPROPERTY()
	FSoundSaveData SoundSaveData;
};
