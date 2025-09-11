// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_StartUpDataPlayer.h"

#include "GameplayAbilitySpec.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/PlayerUIComponent.h"

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

void UDataAsset_StartUpDataPlayer::StartUpDataLevelUp(UTwoMinAbilitySystemComponent* InAscToGive, int32 NewLevel)
{
	if (StartUpDataEffectHandles.IsEmpty()) return;

	// 경험치 저장
	const float SavedExperience = InAscToGive->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentExperienceAttribute());
	
	for (const FActiveGameplayEffectHandle& H : StartUpDataEffectHandles)
	{
		InAscToGive->RemoveActiveGameplayEffect(H);
		//InAscToGive->SetActiveGameplayEffectLevel(H, NewLevel);
	}
	
	StartUpDataEffectHandles.Reset();
	for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
	{
		if (!EffectClass) continue;
		
		const UGameplayEffect* EffectCdo = EffectClass->GetDefaultObject<UGameplayEffect>();
		const FGameplayTagContainer& GameplayTagContainer = EffectCdo->GetAssetTags();
		if (GameplayTagContainer.HasTagExact(TwoMinGameplayTag::GE_Player_Stats) == false)
		{
			continue;
		}
		
		FActiveGameplayEffectHandle H = InAscToGive->ApplyGameplayEffectToSelf(
			EffectCdo,
			NewLevel,
			InAscToGive->MakeEffectContext()
		);
		
		StartUpDataEffectHandles.Add(H);
	}

	if (SavedExperience <= 0.0f) return;
	
	// 경험치 복구.
	InAscToGive->ApplyModToAttribute(UTwoMinAttributeSet::GetCurrentExperienceAttribute(),
		EGameplayModOp::Additive, SavedExperience);

	const float MaxExperience = InAscToGive->GetNumericAttribute(UTwoMinAttributeSet::GetMaxExperienceAttribute());
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(InAscToGive->GetAvatarActor());
	UPlayerUIComponent* PlayerUIComponent = Cast<UPlayerUIComponent>(PlayerCharacter->GetPlayerUIComponent());
	
	PlayerUIComponent->OnCurrentExperienceChanged.Broadcast(SavedExperience / MaxExperience);
}