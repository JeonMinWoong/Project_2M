// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinGA_HitReact_Enemy.h"

int32 UTwoMinGA_HitReact_Enemy::GetPlayHitReactMontageNumber(const AActor* OwnerActor, const AActor* AttackerActor,
	const FAttackInfoData& AttackInfoData) const
{
	const int HitMontageNumber = Super::GetPlayHitReactMontageNumber(OwnerActor, AttackerActor, AttackInfoData);
	if (!HitReactMontages.Contains(HitMontageNumber))
	{
		return 5; // Default Hit Montage Number
	}

	return HitMontageNumber;
}
