// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_ApplyItemConsumeEffect.h"

#include "AbilitySystem/Ability/TwoMinGA_UseItemBase.h"

void UAN_ApplyItemConsumeEffect::FinishAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	Super::FinishAbilityProcess(InAbility);
	
	if (UTwoMinGA_UseItemBase* UseItemAbility = Cast<UTwoMinGA_UseItemBase>(InAbility))
	{
		UseItemAbility->ApplyItemConsumeEffect();
	}
}
