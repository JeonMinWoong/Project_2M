// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_TranslationDistance.h"

void UANS_TranslationDistance::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	bIsTranslation = false;
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_TranslationDistance::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_TranslationDistance::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	bIsTranslation = false;
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
