// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_ToggleAbilityBase.h"

#include "TwoMinDebugHelper.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "Character/TwoMinBaseCharacter.h"

void UANS_ToggleAbilityBase::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!AbilityClass)
	{
		return;
	}

	const ATwoMinBaseCharacter* Owner = Cast<ATwoMinBaseCharacter>(MeshComp->GetOwner());
	if (!Owner) return;

	UTwoMinGameplayAbility* ActiveAbility = Owner->GetAbilitySystemComponent()->GetActiveAbility(AbilityClass);
	if (ActiveAbility)
	{
		StartToggleAbilityProcess(ActiveAbility);
	}
}

void UANS_ToggleAbilityBase::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!AbilityClass)
	{
		return;
	}

	const ATwoMinBaseCharacter* Owner = Cast<ATwoMinBaseCharacter>(MeshComp->GetOwner());
	if (!Owner) return;

	UTwoMinGameplayAbility* ActiveAbility = Owner->GetAbilitySystemComponent()->GetActiveAbility(AbilityClass);
	if (ActiveAbility)
	{
		EndToggleAbilityProcess(ActiveAbility);
	}
}

void UANS_ToggleAbilityBase::StartToggleAbilityProcess(UTwoMinGameplayAbility* InAbility)
{

}

void UANS_ToggleAbilityBase::EndToggleAbilityProcess(UTwoMinGameplayAbility* InAbility)
{

}
