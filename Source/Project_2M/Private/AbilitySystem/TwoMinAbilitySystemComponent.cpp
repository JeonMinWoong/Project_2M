// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TwoMinAbilitySystemComponent.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	if (CancelAndStartNextNewAction(InInputTag)) return;
	
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
			FGameplayTag AbilityTag = GetActiveAbilityTag(Ability->AbilityTags);
			if (IsPlayingAbility(AbilityTag))
			{
				OnCancelAndReTriggerAbility(Ability, Ability->GetClass(), AbilityTag);
			}
			else
			{
				if (Ability->IsMustBeLikedToGameplayAbility()) return;
				
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

bool UTwoMinAbilitySystemComponent::CancelAndStartNextNewAction(const FGameplayTag& InInputTag)
{
	if (UTwoMinGameplayAbility* PlayingAbility = GetPlayingAbility())
	{
		UTwoMinGameplayAbility* NewAbility = GetActiveAbilityInputTag(InInputTag);
		if (PlayingAbility->IsPossibleCancelAbility(NewAbility))
		{
			DebugTwoMin::Print(FString::Printf(TEXT("New Ability: %s, Playing Ability: %s"),
											   *NewAbility->GetName(), *PlayingAbility->GetName()), FColor::Red);
			
			UTwoMinGameplayAbility* CDOAbility =
				Cast<UTwoMinGameplayAbility>(PlayingAbility->GetClass()->GetDefaultObject());
			PlayingAbility->CustomCancelAbility();
			CancelAbility(CDOAbility);

			FTimerDelegate Delegate;
			Delegate.BindLambda([this, NewAbility]()
			{
				this->TryActivateAbilityByClass(NewAbility->GetClass());
			});

			this->GetWorld()->GetTimerManager().SetTimerForNextTick(Delegate);
			return true;
		}
	}
	
	return false;
}

void UTwoMinAbilitySystemComponent::OnCancelAndReTriggerAbility(UTwoMinGameplayAbility* InAbilityCDO,
	const TSubclassOf<UTwoMinGameplayAbility> InAbilityToReTrigger, const FGameplayTag& AbilityTag)
{
 	if (!InAbilityToReTrigger)
	{
		return;
	}

	UTwoMinGameplayAbility* ReTriggerAbility = GetPlayingAbilityTag(AbilityTag);
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

UTwoMinGameplayAbility* UTwoMinAbilitySystemComponent::GetPlayingAbility()
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.IsActive() && AbilitySpec.Ability)
		{
			// CDO 가 없어서 정지가 안되는 듯.
			if (UTwoMinGameplayAbility* TwoMinGameplayAbility = Cast<UTwoMinGameplayAbility>(AbilitySpec.GetPrimaryInstance()))
			{
				if (TwoMinGameplayAbility->GetActivationPolicy() != EToMinAbilityActivationPolicy::OnGiven)
				{
					return TwoMinGameplayAbility;
				}
			}
		}
	}

	return nullptr;
}

bool UTwoMinAbilitySystemComponent::IsPlayingAbility(const FGameplayTag& AbilityTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag) == false)
		{
			continue;
		}
		
		if (AbilitySpec.IsActive() && AbilitySpec.Ability)
		{
			return true;
		}
	}
	
	return false;
}

UTwoMinGameplayAbility* UTwoMinAbilitySystemComponent::GetPlayingAbilityTag(const FGameplayTag& AbilityTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag) == false)
		{
			continue;
		}
		
		if (AbilitySpec.IsActive() && AbilitySpec.Ability)
		{
			if (UTwoMinGameplayAbility* FindAbility = Cast<UTwoMinGameplayAbility>(AbilitySpec.GetPrimaryInstance()))
			{
				return FindAbility;	
			}
		}
	}
	
	return nullptr;
}

FGameplayTag UTwoMinAbilitySystemComponent::GetActiveAbilityTag(FGameplayTagContainer GameplayTagContainer) const
{
	if (GameplayTagContainer.IsEmpty())
	{
		return FGameplayTag::EmptyTag;
	}
	
	return GameplayTagContainer.First();
}

UTwoMinGameplayAbility* UTwoMinAbilitySystemComponent::GetActiveAbilityInputTag(const FGameplayTag& InInputTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		if (AbilitySpec.Ability)
		{
			return Cast<UTwoMinGameplayAbility>(AbilitySpec.GetPrimaryInstance());
		}
	}

	return nullptr;
}

UTwoMinGameplayAbility* UTwoMinAbilitySystemComponent::GetActiveAbility(const FGameplayTag& AbilityTag)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag) == false)
		{
			continue;
		}

		return Cast<UTwoMinGameplayAbility>(AbilitySpec.GetPrimaryInstance());
	}

	return nullptr;
}
