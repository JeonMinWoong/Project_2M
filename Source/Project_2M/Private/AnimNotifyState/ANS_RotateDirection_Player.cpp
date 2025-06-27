// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_RotateDirection_Player.h"

#include "Character/TwoMinPlayerCharacter.h"


void UANS_RotateDirection_Player::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	bIsRotation = false;
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_RotateDirection_Player::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (bIsRotation)
	{
		return;
	}
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacter)
	{
		return;
	}

	FRotator CurrentRotation = PlayerCharacter->GetActorRotation();
	CurrentRotation.Pitch = 0;
	CurrentRotation.Roll = 0;
	
	FVector InputDirection = PlayerCharacter->GetInputDirection();
	if (!InputDirection.IsNearlyZero())
	{
		FRotator TargetDirection = InputDirection.Rotation();
		TargetDirection.Pitch = 0;
		TargetDirection.Roll = 0;

		if (MaxRotationAngle > 0.f)
		{
			float HalfValue = MaxRotationAngle * 0.5f;
			float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetDirection.Yaw);
			
			if (DeltaYaw > HalfValue)
			{
				TargetDirection.Yaw = CurrentRotation.Yaw + HalfValue;
			}
			else if (DeltaYaw < -HalfValue)
			{
				TargetDirection.Yaw = CurrentRotation.Yaw - HalfValue;
			}	
		}

		bIsRotation = true;
		PlayMotionWarpingRotator(PlayerCharacter, TargetDirection);
	}
	else
	{
		bIsRotation = true;
		PlayMotionWarpingRotator(PlayerCharacter, CurrentRotation);
	}
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_RotateDirection_Player::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	bIsRotation = false;
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}