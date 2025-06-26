// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_RotateDirection_Player.h"

#include "MotionWarpingComponent.h"
#include "TwoMinDebugHelper.h"
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

	FVector InputDirection = PlayerCharacter->GetInputDirection();
	if (InputDirection.IsNearlyZero())
	{
		return;	
	}
	
	FRotator TargetDirection = InputDirection.Rotation();
	TargetDirection.Pitch = 0;
	TargetDirection.Roll = 0;

	// FString DebugString2 = FString::Printf(TEXT("목표 %f"), TargetDirection.Yaw);
	// DebugTwoMin::Print(DebugString2, FColor::Purple);
	
	FRotator CurrentRotation = PlayerCharacter->GetActorRotation();
	CurrentRotation.Pitch = 0;
	CurrentRotation.Roll = 0;

	if (MaxRotationAngle > 0.f)
	{
		float HalfValue = MaxRotationAngle * 0.5f;
		float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetDirection.Yaw);

		// FString DebugString3 = FString::Printf(TEXT("차이 %f"), DeltaYaw);
		// DebugTwoMin::Print(DebugString3, FColor::Blue);
		if (DeltaYaw > HalfValue)
		{
			TargetDirection.Yaw = CurrentRotation.Yaw + HalfValue;
			// FString DebugString = FString::Printf(TEXT("오른쪽으로 %f 만큼 넘음"), DeltaYaw - HalfValue);
			// DebugTwoMin::Print(DebugString, FColor::Yellow);
		}
		else if (DeltaYaw < -HalfValue)
		{
			TargetDirection.Yaw = CurrentRotation.Yaw - HalfValue;
			// FString DebugString = FString::Printf(TEXT("왼쪽으로 %f 만큼 넘음"), HalfValue + DeltaYaw);
			// DebugTwoMin::Print(DebugString, FColor::Red);
		}	
	}

	UMotionWarpingComponent* MotionWarpingComponent = PlayerCharacter->GetMotionWarpingComponent();
	if (!MotionWarpingComponent)
	{
		return;
	}

	bIsRotation = true;
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("RotationDirection"),
		PlayerCharacter->GetActorLocation(), TargetDirection);

	// FString DebugString = FString::Printf(TEXT("기존 회전 %f, 최종 회전 %f"), CurrentRotation.Yaw, TargetDirection.Yaw);
	// DebugTwoMin::Print(DebugString, FColor::Green);
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_RotateDirection_Player::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	bIsRotation = false;
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
