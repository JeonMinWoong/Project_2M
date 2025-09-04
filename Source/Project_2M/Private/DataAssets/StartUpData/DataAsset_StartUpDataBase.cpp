// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"

#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"


void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UTwoMinAbilitySystemComponent* InAscToGive,
                                                              int32 ApplyLevel)
{
	GrantAbilities(ActivateOnGivenAbilities, InAscToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InAscToGive, ApplyLevel);

	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			if (!EffectClass) continue;

			const UGameplayEffect* EffectCdo = EffectClass->GetDefaultObject<UGameplayEffect>();
			InAscToGive->ApplyGameplayEffectToSelf(
				EffectCdo,
				ApplyLevel,
				InAscToGive->MakeEffectContext()
			);
		}
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UTwoMinGameplayAbility>>& InAbilitiesToGive,
	UTwoMinAbilitySystemComponent* InAscToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty()) return;

	for (const TSubclassOf<UTwoMinGameplayAbility> Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InAscToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InAscToGive->GiveAbility(AbilitySpec);
	}
}
