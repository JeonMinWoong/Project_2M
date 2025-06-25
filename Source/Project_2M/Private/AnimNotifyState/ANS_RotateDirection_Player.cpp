// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_RotateDirection_Player.h"

#include "Character/TwoMinPlayerCharacter.h"
#include "Types/AttributeStorage.h"

void UANS_RotateDirection_Player::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacter)
	{
		return;
	}

	FVector InputDirection = PlayerCharacter->GetInputDirection();
	if (InputDirection.IsNearlyZero())
	{
		return;	
	}
	
	FRotator TargetRotation = InputDirection.Rotation();
	FRotator NewRotation = FMath::RInterpTo(PlayerCharacter->GetActorRotation(), TargetRotation, FrameDeltaTime,
		RotationSpeed);
	
	PlayerCharacter->SetActorRotation(NewRotation);
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}
