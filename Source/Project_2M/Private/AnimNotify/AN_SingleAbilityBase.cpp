// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_SingleAbilityBase.h"

#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "Character/TwoMinBaseCharacter.h"

void UAN_SingleAbilityBase::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!AbilityClass)
	{
		return;
	}

	const ATwoMinBaseCharacter* Owner = Cast<ATwoMinBaseCharacter>(MeshComp->GetOwner());
	if (!Owner) return;
	
	UTwoMinGameplayAbility* ActiveAbility = Owner->GetAbilitySystemComponent()->GetActiveAbility(AbilityClass);
	if (ActiveAbility)
	{
		FinishAbilityProcess(ActiveAbility);
	}
}

void UAN_SingleAbilityBase::FinishAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	
}
