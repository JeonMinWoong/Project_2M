// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/WorldStageManager.h"

void UWorldStageManager::InitStage()
{
	if (WorldStageNames.IsEmpty()) return;

	for (auto StageName : WorldStageNames)
	{
		WorldStageMap.Add(StageName, false);
	}
	
	if (bIsTestClear)
	{
		if (TestClearArray.IsEmpty()) return;
		
		for (int32 Index = 0; Index < TestClearArray.Num(); ++Index)
		{
			WorldStageMap[WorldStageNames[Index]] = TestClearArray[Index]; 
		}
	}
	else
	{
		FString FirstStageName = WorldStageNames[0];
		WorldStageMap[FirstStageName] = true;	
	}
}
