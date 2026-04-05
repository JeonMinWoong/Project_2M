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
					CharacterToTargeting(PlayerCharacter, AutoTargetingComp);
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
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacter)
	{
		return;
	}
	
	PlayerCharacter->SetStartRotationTargetInfo(nullptr, 0);
	
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

void UANS_RotateDirection_Player::CharacterToTargeting(ATwoMinPlayerCharacter* PlayerCharacter,
	const UAutoTargetingComponent* AutoTargetingComp)
{
	UMotionWarpingComponent* MotionWarpingComp = PlayerCharacter->GetMotionWarpingComponent();
	if (!MotionWarpingComp) return;
	
	MotionWarpingComp->RemoveWarpTarget("RotationDirection");

	bool IsLockOnTarget =
		UTwoMinFunctionLibrary::HasGameplayTag(PlayerCharacter, TwoMinGameplayTag::Player_State_LockOn);
	
	AActor* Target = IsLockOnTarget ? GetLockOnTarget(PlayerCharacter) : AutoTargetingComp->GetCurrentTargetingActor();
	
	if (!Target) return;
	
	PlayerCharacter->SetStartRotationTargetInfo(Target, AutoTargetingComp->GetTargetingRotationSpeed());
}

AActor* UANS_RotateDirection_Player::GetLockOnTarget(ATwoMinBaseCharacter* MyActor)
{
	UTwoMinGameplayAbility* Ability =
		MyActor->GetAbilitySystemComponent()->GetPlayingAbilityTag(TwoMinGameplayTag::Player_Ability_LockOn);
	if (!Ability)
	{
		return nullptr;
	}

	const UTwoMinGA_LockOn_Player* LockOnAbility = Cast<UTwoMinGA_LockOn_Player>(Ability);
	if (!LockOnAbility)
	{
		return nullptr;
	}

	AActor* Target = LockOnAbility->GetCurrentLockOnTarget();
	if (!Target)
	{
		return nullptr;
	}

	return Target;
}
