// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinGA_HitReact_Enemy.h"

int32 UTwoMinGA_HitReact_Enemy::GetPlayHitReactMontageNumber(const FVector OwnerForward, const FVector ToImpact,
	const FAttackInfoData& AttackInfoData) const
{
	const int HitMontageNumber = Super::GetPlayHitReactMontageNumber(OwnerForward, ToImpact, AttackInfoData);
	if (!HitReactMontages.Contains(HitMontageNumber))
	{
		return 5; // Default Hit Montage Number
	}

	return HitMontageNumber;
}