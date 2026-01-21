// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_SlowMotion.h"

void UANS_SlowMotion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	MeshComp->GetWorld()->GetWorldSettings()->SetTimeDilation(SlowMotionRate);
}

void UANS_SlowMotion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	MeshComp->GetWorld()->GetWorldSettings()->SetTimeDilation(OriginalSlowMotionRate);
}
