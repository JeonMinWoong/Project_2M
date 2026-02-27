// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
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
	void SaveGameData(const FSaveGameData& NewSaveGameData);
	FSaveGameData LoadGameData() const;
	bool IsExistSaveGameData() const;

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
};
