// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "WorldStageManager.generated.h"

struct FWorldStageNameData;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class PROJECT_2M_API UWorldStageManager : public UObject
{
	GENERATED_BODY()
	
public:
	void InitStage();
	int32 GetWorldStageIndex(const FString& WorldRealStageName);
	FWorldStageClearGainData* GetCurrentWorldClearStageData(const FString& WorldRealStageName) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "StageInfo|TestClearState")
	bool bIsTestClear = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "StageInfo|TestClearStage")
	TArray<bool> TestClearArray;
	
	UPROPERTY(EditDefaultsOnly, Category = "StageInfo|WorldStageNames")
	TArray<FWorldStageNameData> WorldStageNameDataGroup;
	
	UPROPERTY(VisibleAnywhere, Category = "StageInfo|WorldStageMap")
	TMap<FString, bool> WorldStageMap;
	
	UPROPERTY(EditDefaultsOnly, Category = "StageInfo|ClearGainTable")
	UDataTable* ClearStageTable;
	
	UPROPERTY(EditDefaultsOnly, Category = "StageInfo|Village")
	FString VillageName;

public:
	FORCEINLINE FString GetIndexRealStageName(int32 IndexValue)
	{
		if (!WorldStageNameDataGroup.IsValidIndex(IndexValue)) return "";
		
		return WorldStageNameDataGroup[IndexValue].RealStageName;
	}
	
	FORCEINLINE FString GetIndexShowStageName(int32 IndexValue)
	{
		if (!WorldStageNameDataGroup.IsValidIndex(IndexValue)) return "";
		
		return WorldStageNameDataGroup[IndexValue].ShowStateName;
	}
	FORCEINLINE TMap<FString, bool> GetWorldStageMap() { return WorldStageMap; }
	
	FORCEINLINE void SetWorldStage(FString WorldStageName, const bool bIsClear) 
	{ WorldStageMap.Add(WorldStageName, bIsClear); }
	
	FORCEINLINE bool GetWorldStage(FString WorldStageName) const 
	{ return WorldStageMap.FindRef(WorldStageName); }
	
	FORCEINLINE FString GetVillageName() { return VillageName; }
};
