// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_StartUpDataEnemy.generated.h"

class UTwoMinEnemyGameplayAbility;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UDataAsset_StartUpDataEnemy : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UTwoMinAbilitySystemComponent* InAscToGive,
		int32 ApplyLevel = 1) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<TSubclassOf<UTwoMinEnemyGameplayAbility>> EnemyStartUpAbilities;

};
