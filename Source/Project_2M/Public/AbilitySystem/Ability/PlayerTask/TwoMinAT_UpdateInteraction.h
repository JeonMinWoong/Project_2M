// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Task/TwoMinAbilityTask.h"
#include "TwoMinAT_UpdateInteraction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionOnTickDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinAT_UpdateInteraction : public UTwoMinAbilityTask
{
	GENERATED_BODY()
	
public:
	static UTwoMinAT_UpdateInteraction* CreateTickTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	
	FInteractionOnTickDelegate OnInteractionTick;
	
};
