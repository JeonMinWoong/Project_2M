// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/WorldStageManager.h"

void UWorldStageManager::InitStage()
{
	if (WorldStageNameDataGroup.IsEmpty()) return;

	for (auto StageData : WorldStageNameDataGroup)
	{
		if (StageData.RealStageName == "DevelopMap") continue;
		
		WorldStageMap.Add(StageData.RealStageName, false);
	}
	
	if (bIsTestClear)
	{
		if (TestClearArray.IsEmpty()) return;
		
		for (int32 Index = 1; Index <= TestClearArray.Num(); ++Index)
		{
			WorldStageMap[WorldStageNameDataGroup[Index].RealStageName] = TestClearArray[Index - 1]; 
		}
	}
	else
	{
		FString FirstStageName = WorldStageNameDataGroup[1].RealStageName;
		WorldStageMap[FirstStageName] = true;	
	}
}

int32 UWorldStageManager::GetWorldStageIndex(const FString& WorldRealStageName)
{
	for (int32 Index = 0; Index < WorldStageNameDataGroup.Num(); ++Index)
	{
		if (WorldStageNameDataGroup[Index].RealStageName == WorldRealStageName)
		{
			return Index;
		}
	}
	
	return -1;
}

FWorldStageClearGainData* UWorldStageManager::GetCurrentWorldClearStageData(const FString& WorldRealStageName) const
{
	TArray<FWorldStageClearGainData*> ClearStageTableGroup;
		
	ClearStageTable->GetAllRows(WorldRealStageName, ClearStageTableGroup);
		
	for (FWorldStageClearGainData* ClearStateData : ClearStageTableGroup)
	{
		if (ClearStateData && ClearStateData->ClearItemOwner == WorldRealStageName)
		{
			return ClearStateData;
		}
	}
	
	return nullptr;
}

void UWorldStageManager::ClearCurrentWorldStage()
{
	const FString CurRealStageName = GetWorld()->RemovePIEPrefix(GetWorld()->GetMapName());
	const int32 NextIndex = GetWorldStageIndex(CurRealStageName) + 1;
	if (NextIndex <= 1) return;
	
	const FString NextRealStageName = GetIndexRealStageName(NextIndex);
	if (NextRealStageName.IsEmpty()) return;
	if (WorldStageMap.Contains(NextRealStageName) == false) return;
	
	WorldStageMap[NextRealStageName] = true;
}
