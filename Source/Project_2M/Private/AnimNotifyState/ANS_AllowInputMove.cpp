// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_AllowInputMove.h"

#include "Character/TwoMinPlayerCharacter.h"

void UANS_AllowInputMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ATwoMinPlayerCharacter* Onwer = Cast<ATwoMinPlayerCharacter>(MeshComp->GetOwner());
	if (!Onwer) return;

	Onwer->AddMovePossibleCancelAbility(PlayingAbilityTag);
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_AllowInputMove::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	ATwoMinPlayerCharacter* Onwer = Cast<ATwoMinPlayerCharacter>(MeshComp->GetOwner());
	if (!Onwer) return;

	Onwer->RemoveMovePossibleCancelAbility(PlayingAbilityTag);
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
