// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_TA_CancelAndReTriggerSelf.h"

#include "AbilitySystem/Ability/Player/TwoMinGA_Roll_Player.h"

void UANS_TA_CancelAndReTriggerSelf::StartToggleAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	InAbility->SetReTriggerActive(true);
}

void UANS_TA_CancelAndReTriggerSelf::EndToggleAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	InAbility->SetReTriggerActive(false);
}
