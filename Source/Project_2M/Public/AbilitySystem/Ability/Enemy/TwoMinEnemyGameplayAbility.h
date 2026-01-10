// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinEnemyGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinEnemyGameplayAbility : public UTwoMinGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnStartTeleport(float TeleportDistance);
	
	UFUNCTION()
	void OnFinishTeleport();
};
