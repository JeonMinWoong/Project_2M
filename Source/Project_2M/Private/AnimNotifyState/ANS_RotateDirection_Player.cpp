// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_RotateDirection_Player.h"

#include "MotionWarpingComponent.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "AbilitySystem/Ability/Player/TwoMinGA_LockOn_Player.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/AutoTargetingComponent.h"
#include "Compnents/Combat/BaseCombatComponent.h"
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
			{
				if (PlayerCharacter->GetCurrentAutoTarget())
				{
					const UAutoTargetingComponent* AutoTargetingComp =
						PlayerCharacter->GetCombatComponent()->GetAutoTargetingComponent();
					CharacterToTargeting(PlayerCharacter, AutoTargetingComp, FrameDeltaTime);
				}
				else
				{
					CharacterToInputDirection(PlayerCharacter);	
				}
			}
		
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
	if (UTwoMinFunctionLibrary::HasGameplayTag(PlayerCharacter, TwoMinGameplayTag::Player_State_LockOn))
	{
		// Lock On 중 회전 하지 않음.
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
}

void UANS_RotateDirection_Player::CharacterToTargetDirection(ATwoMinPlayerCharacter* PlayerCharacter,
	float FrameDeltaTime)
{
	UTwoMinGameplayAbility* Ability =
		PlayerCharacter->GetAbilitySystemComponent()->GetPlayingAbilityTag(TwoMinGameplayTag::Player_Ability_LockOn);
	if (!Ability)
	{
		return;
	}

	const UTwoMinGA_LockOn_Player* LockOnAbility = Cast<UTwoMinGA_LockOn_Player>(Ability);
	if (!LockOnAbility)
	{
		return;
	}

	const AActor* Target = LockOnAbility->GetCurrentLockOnTarget();
	if (!Target)
	{
		return;
	}

	const FRotator LockOnRotator = (Target->GetActorLocation() - PlayerCharacter->GetActorLocation()).Rotation();
	const FRotator NewCharacterRot = FMath::RInterpTo(PlayerCharacter->GetActorRotation(),
	FRotator(0.f, LockOnRotator.Yaw, 0.f), FrameDeltaTime, 10);
	
	PlayerCharacter->SetActorRotation(NewCharacterRot);
}

void UANS_RotateDirection_Player::CharacterToTargeting(ATwoMinPlayerCharacter* PlayerCharacter,
	const UAutoTargetingComponent* AutoTargetingComp, const float FrameDeltaTime)
{
	UMotionWarpingComponent* MotionWarpingComp = PlayerCharacter->GetMotionWarpingComponent();
	if (!MotionWarpingComp) return;
	
	MotionWarpingComp->RemoveWarpTarget("RotationDirection");
	
	const AActor* Target = AutoTargetingComp->GetCurrentTargetingActor();
	const float TargetingRotationSpeed = AutoTargetingComp->GetTargetingRotationSpeed();
	if (!Target) return;

	const FRotator TargetDirection = (Target->GetActorLocation() - PlayerCharacter->GetActorLocation()).Rotation();
	const FRotator NewCharacterRot = FMath::RInterpTo(PlayerCharacter->GetActorRotation(),
	FRotator(0.f, TargetDirection.Yaw, 0.f), FrameDeltaTime, TargetingRotationSpeed);
	
	PlayerCharacter->SetActorRotation(NewCharacterRot);
}
