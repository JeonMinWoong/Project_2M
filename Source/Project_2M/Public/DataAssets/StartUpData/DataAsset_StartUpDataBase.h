// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UToMinAbilitySystemComponent;
class UToMinGameplayAbility;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UToMinAbilitySystemComponent* InAscToGive, int32 ApplyLevel = 1);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UToMinGameplayAbility>> ActivateOnGivenAbilities;

	void GrantAbilities(const TArray<TSubclassOf<UToMinGameplayAbility>>& InAbilitiesToGive,
		UToMinAbilitySystemComponent* InAscToGive, int32 ApplyLevel = 1); 
};
