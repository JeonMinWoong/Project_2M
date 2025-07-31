// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_StartConversionCombo.h"

#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"


void UANS_StartConversionCombo::NewAbilityCustomStartProcess(const ATwoMinBaseCharacter* Owner,
                                                             UTwoMinGameplayAbility* NewAbility)
{
	UTwoMinGA_AttackBase* Attack = Cast<UTwoMinGA_AttackBase>(NewAbility);
	if (Attack)
	{
		Attack->ConversionComboCount(ConversionComboCount);
	}
}

void UANS_StartConversionCombo::NewAbilityCustomEndProcess(const ATwoMinBaseCharacter* Owner,
	UTwoMinGameplayAbility* NewAbility)
{
	UTwoMinGA_AttackBase* Attack = Cast<UTwoMinGA_AttackBase>(NewAbility);
	if (Attack)
	{
		Attack->ResetComboCount();
	}
}
