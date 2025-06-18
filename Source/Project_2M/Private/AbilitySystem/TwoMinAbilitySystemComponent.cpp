// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TwoMinAbilitySystemComponent.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		if (InInputTag.MatchesTag(TwoMinGameplayTag::InputTag_Toggle) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
		else
		{
			UTwoMinGameplayAbility* Ability = Cast<UTwoMinGameplayAbility>(AbilitySpec.Ability);
			if (IsAbilityActive(Ability->GetClass()))
			{
				OnCancelAndReTriggerAbility(Ability, Ability->GetClass());
			}
			else
			{
				TryActivateAbility(AbilitySpec.Handle);	
			}
		}
	}
}

void UTwoMinAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(TwoMinGameplayTag::InputTag_MustBeHold)) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

void UTwoMinAbilitySystemComponent::OnCancelAndReTriggerAbility(UTwoMinGameplayAbility* InAbilityCDO,
	const TSubclassOf<UTwoMinGameplayAbility> InAbilityToReTrigger)
{
 	if (!InAbilityToReTrigger)
	{
		return;
	}

	UTwoMinGameplayAbility* ReTriggerAbility = GetActiveAbility(InAbilityToReTrigger);
	if (ReTriggerAbility->GetAbilityInputType() == ETwoAbilityInputType::Only)
	{
		return;
	}
	
	if (!ReTriggerAbility->IsReTriggerActive())
	{
		return;
	}
	
	CancelAbility(Cast<UGameplayAbility>(InAbilityCDO));

	FTimerDelegate Delegate;
	Delegate.BindLambda([this, InAbilityToReTrigger]()
	{
		this->TryActivateAbilityByClass(InAbilityToReTrigger);
		Debug::Print(TEXT("Delegate ReTrigger Ability"), FColor::Yellow);
	});

	this->GetWorld()->GetTimerManager().SetTimerForNextTick(Delegate);
}

void UTwoMinAbilitySystemComponent::GrantHeroWeaponAbilities(
	const TArray<FTwoMinPlayerAbilitySet>& InDefaultWeaponAbilities, int ApplyLevel,
	TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty()) return;

	for (const FTwoMinPlayerAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

UTwoMinGameplayAbility* UTwoMinAbilitySystemComponent::GetActiveAbility(
	const TSubclassOf<UTwoMinGameplayAbility> AbilityClass)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.IsActive() && AbilitySpec.Ability && AbilitySpec.Ability->GetClass() == AbilityClass)
		{
			if (UTwoMinGameplayAbility* FindAbility = Cast<UTwoMinGameplayAbility>(AbilitySpec.GetPrimaryInstance()))
			{
				return FindAbility;	
			}
		}
	}
	
	return nullptr;
}

bool UTwoMinAbilitySystemComponent::IsAbilityActive(const TSubclassOf<UTwoMinGameplayAbility> AbilityClass)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.IsActive() && AbilitySpec.Ability && AbilitySpec.Ability->GetClass() == AbilityClass)
		{
			return true;
		}
	}
	
	return false;
}
