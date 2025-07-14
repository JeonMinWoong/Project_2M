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
	InAbility->AddPossibleCancelAbility(NewAbility);

	DebugTwoMin::Print(TEXT("공격 입력 가능"), FColor::Green);
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
	InAbility->RemovePossibleCancelAbility(NewAbility);

	DebugTwoMin::Print(TEXT("공격 입력 불가능"), FColor::Green);
}
