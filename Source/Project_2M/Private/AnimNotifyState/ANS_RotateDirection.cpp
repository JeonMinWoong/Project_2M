// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_RotateDirection.h"

void UANS_RotateDirection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}
