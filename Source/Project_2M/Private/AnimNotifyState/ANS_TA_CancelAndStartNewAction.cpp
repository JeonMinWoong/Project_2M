// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_TA_CancelAndStartNewAction.h"

#include "TwoMinDebugHelper.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "Character/TwoMinBaseCharacter.h"

void UANS_TA_CancelAndStartNewAction::StartToggleAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	if (NewAbilityTag == FGameplayTag::EmptyTag)
	{
		return;
	}

	const ATwoMinBaseCharacter* Owner = Cast<ATwoMinBaseCharacter>(InAbility->GetAvatarActorFromActorInfo());
	if (!Owner) return;
	
	UTwoMinGameplayAbility* NewAbility = Owner->GetAbilitySystemComponent()->GetActiveAbility(NewAbilityTag);
	NewAbilityCustomStartProcess(Owner, NewAbility);
	InAbility->AddPossibleCancelAbility(NewAbility);
}

void UANS_TA_CancelAndStartNewAction::EndToggleAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	if (NewAbilityTag == FGameplayTag::EmptyTag)
	{
		return;
	}

	const ATwoMinBaseCharacter* Owner = Cast<ATwoMinBaseCharacter>(InAbility->GetAvatarActorFromActorInfo());
	if (!Owner) return;
	
	UTwoMinGameplayAbility* NewAbility = Owner->GetAbilitySystemComponent()->GetActiveAbility(NewAbilityTag);
	NewAbilityCustomEndProcess(Owner, NewAbility);
	InAbility->RemovePossibleCancelAbility(NewAbility);
}

void UANS_TA_CancelAndStartNewAction::NewAbilityCustomStartProcess(const ATwoMinBaseCharacter* Owner,
	UTwoMinGameplayAbility* NewAbility)
{
	
}

void UANS_TA_CancelAndStartNewAction::NewAbilityCustomEndProcess(const ATwoMinBaseCharacter* Owner,
	UTwoMinGameplayAbility* NewAbility)
{
	
}
