// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_SA_ComboFinish.h"

#include "TwoMinDebugHelper.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"

void UAN_SA_ComboFinish::FinishAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	if (UTwoMinGA_AttackBase* Attack = Cast<UTwoMinGA_AttackBase>(InAbility))
	{
		Attack->ResetComboCount();
	}
}
