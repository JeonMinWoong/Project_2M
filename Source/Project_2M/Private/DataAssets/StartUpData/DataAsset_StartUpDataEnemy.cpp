// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_StartUpDataEnemy.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEnemyGameplayAbility.h"

void UDataAsset_StartUpDataEnemy::GiveToAbilitySystemComponent(UTwoMinAbilitySystemComponent* InAscToGive,
                                                               int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InAscToGive, ApplyLevel);

	if (!EnemyStartUpAbilities.IsEmpty())
	{
		for (const TSubclassOf<UTwoMinEnemyGameplayAbility>& AbilityClass : EnemyStartUpAbilities)	
		{
			if (!AbilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = InAscToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;

			InAscToGive->GiveAbility(AbilitySpec);
		}
	}
}
