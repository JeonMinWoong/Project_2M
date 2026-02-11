// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/WorldStageManager.h"

void UWorldStageManager::InitStage()
{
	if (WorldStageNames.IsEmpty()) return;

	for (auto StageName : WorldStageNames)
	{
		if (StageName == "DevelopMap") continue;
		
		WorldStageMap.Add(StageName, false);
	}
	
	if (bIsTestClear)
	{
		if (TestClearArray.IsEmpty()) return;
		
		for (int32 Index = 1; Index < TestClearArray.Num(); ++Index)
		{
			WorldStageMap[WorldStageNames[Index]] = TestClearArray[Index - 1]; 
		}
	}
	else
	{
		FString FirstStageName = WorldStageNames[1];
		WorldStageMap[FirstStageName] = true;	
	}
}
