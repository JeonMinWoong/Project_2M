// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify/AN_SingleAbilityBase.h"
#include "AN_SA_CachedTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UAN_SA_CachedTargetLocation : public UAN_SingleAbilityBase
{
	GENERATED_BODY()
	
protected:
	virtual void FinishAbilityProcess(UTwoMinGameplayAbility* InAbility);

	UPROPERTY(EditAnywhere, Category = "SendGameplayEventToSelf")
	FGameplayTag EventTag;
	
};
