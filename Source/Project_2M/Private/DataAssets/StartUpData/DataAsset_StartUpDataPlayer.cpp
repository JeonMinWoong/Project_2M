// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_StartUpDataPlayer.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"

void UDataAsset_StartUpDataPlayer::GiveToAbilitySystemComponent(UTwoMinAbilitySystemComponent* InAscToGive,
                                                                int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InAscToGive, ApplyLevel);

	for (const FTwoMinPlayerAbilitySet& AbilitySet : PlayerStartUpAbilitySets)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = InAscToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		
		InAscToGive->GiveAbility(AbilitySpec);
	}
}
