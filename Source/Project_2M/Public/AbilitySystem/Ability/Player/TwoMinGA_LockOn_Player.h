// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_LockOn_Player.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_LockOn_Player : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

private:
	AActor* FindLockOnTarget();
	void CheckSphereOverTargetGroup();
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float LockOnDistance = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings",
		meta=(ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float LockOnAngle = 45.f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float LockOnSwitchTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|Debug")
	bool bIsDebugLockOnRange = false; 
	UPROPERTY()
	TArray<AActor*> LockOnTargetGroup;

	UPROPERTY()
	AActor* LockOnTarget;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
};
