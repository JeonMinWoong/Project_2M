// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_SA_AddComboCount.h"

#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEGA_AttackBase.h"

void UAN_SA_AddComboCount::FinishAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	if (UTwoMinEGA_AttackBase* EnemyAttack = Cast<UTwoMinEGA_AttackBase>(InAbility))
	{
		EnemyAttack->AddComboCount();
	}
}
