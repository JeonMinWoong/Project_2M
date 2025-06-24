// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_WeaponCollision.h"

#include "Character/TwoMinBaseCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"

void UANS_WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ATwoMinBaseCharacter* Onwer = Cast<ATwoMinBaseCharacter>(MeshComp->GetOwner());
	if (!Onwer)
	{
		return;
	}

	Onwer->GetCombatComponent()->ToggleWeaponCollision(true, ToggleDamageType);
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	ATwoMinBaseCharacter* Onwer = Cast<ATwoMinBaseCharacter>(MeshComp->GetOwner());
	if (!Onwer)
	{
		return;
	}

	Onwer->GetCombatComponent()->ToggleWeaponCollision(false, ToggleDamageType);
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
