// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"

#include "AbilitySystem/ToMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/ToMinGameplayAbility.h"


void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UToMinAbilitySystemComponent* InAscToGive,
	int32 ApplyLevel)
{
	GrantAbilities(ActivateOnGivenAbilities, InAscToGive, ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UToMinGameplayAbility>>& InAbilitiesToGive,
	UToMinAbilitySystemComponent* InAscToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty()) return;

	for (const TSubclassOf<UToMinGameplayAbility> Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InAscToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InAscToGive->GiveAbility(AbilitySpec);
	}
}
