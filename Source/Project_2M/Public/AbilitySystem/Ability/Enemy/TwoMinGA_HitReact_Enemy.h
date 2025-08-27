// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_HitReactBase.h"
#include "TwoMinGA_HitReact_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_HitReact_Enemy : public UTwoMinGA_HitReactBase
{
	GENERATED_BODY()

protected:
	virtual int32 GetPlayHitReactMontageNumber(const FVector OwnerForward, const FVector ToImpact,
		const FAttackInfoData& AttackInfoData) const override;
};
