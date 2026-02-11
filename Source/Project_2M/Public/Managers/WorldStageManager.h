// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldStageManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class PROJECT_2M_API UWorldStageManager : public UObject
{
	GENERATED_BODY()
	
public:
	void InitStage();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "TestClearState")
	bool bIsTestClear = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "TestClearStage")
	TArray<bool> TestClearArray;
	
	UPROPERTY(EditDefaultsOnly, Category = "WorldStageNames")
	TArray<FString> WorldStageNames;
	
	UPROPERTY(VisibleAnywhere)
	TMap<FString, bool> WorldStageMap;
	
public:
	FORCEINLINE FString GetCurrentStageName(int32 IndexValue) const { return WorldStageNames[IndexValue]; }
	FORCEINLINE TMap<FString, bool> GetWorldStageMap() { return WorldStageMap; }
	
	FORCEINLINE void SetWorldStage(FString WorldStageName, const bool bIsClear) 
	{ WorldStageMap.Add(WorldStageName, bIsClear); }
	
	FORCEINLINE bool GetWorldStage(FString WorldStageName) const 
	{ return WorldStageMap.FindRef(WorldStageName); }
};
