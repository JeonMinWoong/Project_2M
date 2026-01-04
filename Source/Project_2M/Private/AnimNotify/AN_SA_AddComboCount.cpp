// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_SA_AddComboCount.h"

#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEGA_AttackBase.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEGA_SpecialAttackBase.h"
#include "AbilitySystem/Ability/Player/TwoMinGA_SpecialAttackBase.h"

void UAN_SA_AddComboCount::FinishAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	if (UTwoMinEGA_AttackBase* EnemyAttack = Cast<UTwoMinEGA_AttackBase>(InAbility))
	{
		EnemyAttack->AddComboCount();
	}
	
	if (UTwoMinGA_SpecialAttackBase* SpecialAttack = Cast<UTwoMinGA_SpecialAttackBase>(InAbility))
	{
		SpecialAttack->AddComboCount();
	}
	
	if (UTwoMinEGA_SpecialAttackBase* EnemySpecialAttack = Cast<UTwoMinEGA_SpecialAttackBase>(InAbility))
	{
		EnemySpecialAttack->AddComboCount();
	}
}
