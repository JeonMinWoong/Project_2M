// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"
#include "TwoMinGA_Guard_Player.generated.h"

class UTwoMinAT_UpdateRotation;

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_Guard_Player : public UTwoMinGA_GuardBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
private:
	UFUNCTION()
	void OnUpdateRotationTick(float DeltaSeconds);
	
	UPROPERTY(EditDefaultsOnly, Category = "RotationTime")
	float RotationTime = 0.1f;

	UPROPERTY()
	float CurTime = 0;
	
	UPROPERTY()
	UTwoMinAT_UpdateRotation* UpdateRotationTickTask;
	
};
