// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_ModifyMontagePlayRate.h"

void UANS_ModifyMontagePlayRate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;

	MontageInstance = AnimInstance->GetActiveMontageInstance();
	if (!MontageInstance) return;

	PreviousPlayRate = MontageInstance->GetPlayRate();
	MontageInstance->SetPlayRate(PlayRate);
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_ModifyMontagePlayRate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;
	
	if (!MontageInstance) return;
	
	MontageInstance->SetPlayRate(PreviousPlayRate);
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
