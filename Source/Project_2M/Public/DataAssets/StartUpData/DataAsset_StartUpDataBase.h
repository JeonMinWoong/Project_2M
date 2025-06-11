// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UTwoMinAbilitySystemComponent;
class UTwoMinGameplayAbility;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UTwoMinAbilitySystemComponent* InAscToGive, int32 ApplyLevel = 1);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UTwoMinGameplayAbility>> ActivateOnGivenAbilities;

	void GrantAbilities(const TArray<TSubclassOf<UTwoMinGameplayAbility>>& InAbilitiesToGive,
		UTwoMinAbilitySystemComponent* InAscToGive, int32 ApplyLevel = 1); 
};
