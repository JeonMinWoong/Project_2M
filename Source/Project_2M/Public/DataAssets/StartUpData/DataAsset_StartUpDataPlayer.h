// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_StartUpDataPlayer.generated.h"

struct FTwoMinPlayerAbilitySet;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UDataAsset_StartUpDataPlayer : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UTwoMinAbilitySystemComponent* InAscToGive,
		int32 ApplyLevel = 1) override;

	virtual void StartUpDataLevelUp(UTwoMinAbilitySystemComponent* InAscToGive, int32 NewLevel) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FTwoMinPlayerAbilitySet> PlayerStartUpAbilitySets;
};
