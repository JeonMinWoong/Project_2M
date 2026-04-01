// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/TwoMinSaveGame.h"

#include "TwoMinDebugHelper.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinSaveGame::SavePlayerGameData(const FSaveGameData& NewSaveGameData)
{
	WorldStageMap = NewSaveGameData.WorldStageMap;
	PlayerLevel = NewSaveGameData.PlayerLevel;
	PlayerCurrentExp = NewSaveGameData.PlayerCurrentExp;
	PlayerCurrentGold = NewSaveGameData.PlayerCurrentGold;
	PlayerCurrentItems = NewSaveGameData.PlayerCurrentItems;
	//TwoMinDebugHelper::Print(TEXT("저장 중..."), FColor::Green);
}

FSaveGameData UTwoMinSaveGame::LoadPlayerGameData() const
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

bool UTwoMinSaveGame::IsExistSavePlayerGameData() const
{
	return PlayerLevel > 0;
}

void UTwoMinSaveGame::SaveSoundData(const FSoundSaveData& NewSoundSaveData)
{
	SoundSaveData = NewSoundSaveData;
}

FSoundSaveData UTwoMinSaveGame::LoadSoundData() const
{
	return SoundSaveData;
}