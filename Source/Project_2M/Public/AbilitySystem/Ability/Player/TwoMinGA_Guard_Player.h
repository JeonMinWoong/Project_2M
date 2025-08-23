// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"
#include "TwoMinGA_Guard_Player.generated.h"

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
};
