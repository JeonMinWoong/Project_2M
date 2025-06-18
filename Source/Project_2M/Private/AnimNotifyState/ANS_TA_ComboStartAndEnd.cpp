// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_TA_ComboStartAndEnd.h"

#include "TwoMinDebugHelper.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"

void UANS_TA_ComboStartAndEnd::StartToggleAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	if (UTwoMinGA_AttackBase* Attack = Cast<UTwoMinGA_AttackBase>(InAbility))
	{
		Attack->AddComboCount();
		Debug::Print(TEXT("Combo Delay Start"), FColor::Green);
	}
}

void UANS_TA_ComboStartAndEnd::EndToggleAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	if (UTwoMinGA_AttackBase* Attack = Cast<UTwoMinGA_AttackBase>(InAbility))
	{
		Attack->ResetComboCount();
		Debug::Print(TEXT("Combo Count Reset"), FColor::Red);
	}
}
