// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Task/TwoMinAbilityTask.h"
#include "TwoMinAT_TeleportTaskBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartTeleportDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishTeleportDelegate);

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinAT_TeleportTaskBase : public UTwoMinAbilityTask
{
	GENERATED_BODY()
	
public:
	static UTwoMinAT_TeleportTaskBase* CreateTickTask(UGameplayAbility* OwningAbility, float InTeleportStartDelay, 
		float InTeleportFinishDelay);
	
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnStartTeleportDelegate FOnStartTeleport;
	
	UPROPERTY(BlueprintAssignable)
	FOnFinishTeleportDelegate FOnFinishTeleport;
	
private:
	UPROPERTY(EditDefaultsOnly)
	float TeleportStartDelay;
	
	UPROPERTY(EditDefaultsOnly)
	float TeleportFinishDelay;

	UPROPERTY()
	float ElapsedTime;
	
	UPROPERTY()
	bool bIsTeleportStarted;
};
