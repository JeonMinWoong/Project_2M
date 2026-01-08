// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Task/TwoMinAbilityTask.h"
#include "TwoMinAT_UpdatePickUpItem_Player.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickUpItemOnTickDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinAT_UpdatePickUpItem_Player : public UTwoMinAbilityTask
{
	GENERATED_BODY()

public:
	static UTwoMinAT_UpdatePickUpItem_Player* CreateTickTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	
	FPickUpItemOnTickDelegate OnPickUpItemTick;
};
