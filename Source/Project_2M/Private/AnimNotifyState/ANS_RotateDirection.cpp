// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_RotateDirection.h"

#include "MotionWarpingComponent.h"
#include "Character/TwoMinBaseCharacter.h"

class UMotionWarpingComponent;

void UANS_RotateDirection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	bIsRotation = false;
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_RotateDirection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_RotateDirection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	bIsRotation = false;
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UANS_RotateDirection::CharacterToTargetDirection(ATwoMinBaseCharacter* MyActor, float FrameDeltaTime)
{
	const AActor* Target = GetLockOnTarget(MyActor);
	if (!Target) return;

	const FRotator LockOnRotator = (Target->GetActorLocation() - MyActor->GetActorLocation()).Rotation();
	const FRotator NewCharacterRot = FMath::RInterpTo(MyActor->GetActorRotation(),
	FRotator(0.f, LockOnRotator.Yaw, 0.f), FrameDeltaTime, LockTargetRotateSpeed);
	
	MyActor->SetActorRotation(NewCharacterRot);
}

AActor* UANS_RotateDirection::GetLockOnTarget(ATwoMinBaseCharacter* MyActor)
{
	return nullptr;
}

void UANS_RotateDirection::PlayMotionWarpingRotator(ATwoMinBaseCharacter* Character, FRotator TargetDRotator)
{
	UMotionWarpingComponent* MotionWarpingComponent = Character->GetMotionWarpingComponent();
	if (!MotionWarpingComponent)
	{
		return;
	}
	
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("RotationDirection"),
		Character->GetActorLocation(), TargetDRotator);
}
