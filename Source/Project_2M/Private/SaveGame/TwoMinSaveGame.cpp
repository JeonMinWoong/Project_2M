// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/TwoMinSaveGame.h"

#include "TwoMinDebugHelper.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinSaveGame::SaveGameData(const FSaveGameData& NewSaveGameData)
{
	WorldStageMap = NewSaveGameData.WorldStageMap;
	PlayerLevel = NewSaveGameData.PlayerLevel;
	PlayerCurrentExp = NewSaveGameData.PlayerCurrentExp;
	PlayerCurrentGold = NewSaveGameData.PlayerCurrentGold;
	PlayerCurrentItems = NewSaveGameData.PlayerCurrentItems;
	
	//TwoMinDebugHelper::Print(TEXT("저장 중..."), FColor::Green);
}

FSaveGameData UTwoMinSaveGame::LoadGameData() const
{
	FSaveGameData NewSaveGameData;
	NewSaveGameData.WorldStageMap = WorldStageMap;
	NewSaveGameData.PlayerLevel = PlayerLevel;
	NewSaveGameData.PlayerCurrentExp = PlayerCurrentExp;
	NewSaveGameData.PlayerCurrentGold = PlayerCurrentGold;
	NewSaveGameData.PlayerCurrentItems = PlayerCurrentItems;
	
	//TwoMinDebugHelper::Print(TEXT("불러오기 중..."), FColor::Green);
	return NewSaveGameData;
}
