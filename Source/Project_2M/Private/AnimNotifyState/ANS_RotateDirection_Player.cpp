// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_RotateDirection_Player.h"

#include "MotionWarpingComponent.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "AbilitySystem/Ability/Player/TwoMinGA_LockOn_Player.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ToMinTypes/TwoMinEnumTypes.h"


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

	switch (RotateDirectionType)
	{
		case ERotateDirectionType::InputDirection:
			CharacterToInputDirection(PlayerCharacter);
			break;
		case ERotateDirectionType::TargetDirection:
			CharacterToTargetDirection(PlayerCharacter, FrameDeltaTime);
			break;
	}
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_RotateDirection_Player::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	bIsRotation = false;
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UANS_RotateDirection_Player::CharacterToInputDirection(ATwoMinPlayerCharacter* PlayerCharacter)
{
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
}

void UANS_RotateDirection_Player::CharacterToTargetDirection(ATwoMinPlayerCharacter* PlayerCharacter, float FrameDeltaTime)
{
	UMotionWarpingComponent* MotionWarpingComponent = PlayerCharacter->GetMotionWarpingComponent();
	if (!MotionWarpingComponent)
	{
		return;
	}
	
	UTwoMinGameplayAbility* Ability = PlayerCharacter->GetAbilitySystemComponent()->GetPlayingAbilityTag(TwoMinGameplayTag::Player_Ability_LockOn);
	if (!Ability)
	{
		return;
	}
	
	UTwoMinGA_LockOn_Player* LockOnAbility = Cast<UTwoMinGA_LockOn_Player>(Ability);
	if (!LockOnAbility)
	{
		return;
	}
	
	AActor* Target = LockOnAbility->GetCurrentLockOnTarget();
	if (!Target)
	{
		return;
	}

	FRotator LockOnRotator = (Target->GetActorLocation() - PlayerCharacter->GetActorLocation()).Rotation();
	FRotator NewCharacterRot = FMath::RInterpTo(PlayerCharacter->GetActorRotation(),
	FRotator(0.f, LockOnRotator.Yaw, 0.f), FrameDeltaTime, 10);
	PlayerCharacter->SetActorRotation(NewCharacterRot);
}
