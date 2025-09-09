// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_DeathBase.h"
#include "TwoMinGA_Death_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_Death_Enemy : public UTwoMinGA_DeathBase
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~ End UGameplayAbility Interface
};
