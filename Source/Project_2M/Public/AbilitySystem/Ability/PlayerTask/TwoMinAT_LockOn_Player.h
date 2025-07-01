// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinAbilityTask.h"
#include "TwoMinAT_LockOn_Player.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockOnTickDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinAT_LockOn_Player : public UTwoMinAbilityTask
{
	GENERATED_BODY()
	
public:
	static UTwoMinAT_LockOn_Player* CreateTickTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FLockOnTickDelegate OnLockOnTick;
};
