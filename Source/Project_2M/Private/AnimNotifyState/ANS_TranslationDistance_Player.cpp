// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_TranslationDistance_Player.h"

#include "MotionWarpingComponent.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/Player/TwoMinGA_LockOn_Player.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/AutoTargetingComponent.h"
#include "Compnents/Combat/BaseCombatComponent.h"

class UTwoMinGA_LockOn_Player;

void UANS_TranslationDistance_Player::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (bIsTranslation)
	{
		return;
	}

	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacter)
	{
		return;
	}

	switch (TranslationDistanceType)
	{
		case ETranslationDistanceType::InputDistance:
			{
				if (PlayerCharacter->GetCurrentAutoTarget())
				{
					const UAutoTargetingComponent* AutoTargetingComp =
						PlayerCharacter->GetCombatComponent()->GetAutoTargetingComponent();
					CharacterToAutoTargetingDistance(PlayerCharacter, AutoTargetingComp, FrameDeltaTime);
				}
				else
				{
					CharacterToInputDistance(PlayerCharacter);
				}
			}
		
			break;
		case ETranslationDistanceType::TargetDistance:
			break;
		default:
			break;
	}
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_TranslationDistance_Player::CharacterToInputDistance(ATwoMinPlayerCharacter* PlayerCharacter)
{
	FVector CurrentLocation = PlayerCharacter->GetActorLocation();
	FVector InputDirection = PlayerCharacter->GetInputDirection().GetSafeNormal();
	FVector TargetLocation = CurrentLocation + (InputDirection * TranslationDistance);

	bIsTranslation = true;
	PlayMotionWarpingTargetLocation(PlayerCharacter, TargetLocation);
}

void UANS_TranslationDistance_Player::CharacterToAutoTargetingDistance(ATwoMinPlayerCharacter* PlayerCharacter,
	const UAutoTargetingComponent* AutoTargetingComp, const float FrameDeltaTime)
{
	UMotionWarpingComponent* MotionWarpingComp = PlayerCharacter->GetMotionWarpingComponent();
	if (!MotionWarpingComp) return;
	
	MotionWarpingComp->RemoveWarpTarget("TranslationDistance");

	bool IsLockOnTarget =
		UTwoMinFunctionLibrary::HasGameplayTag(PlayerCharacter, TwoMinGameplayTag::Player_State_LockOn);
	
	const AActor* Target =
		IsLockOnTarget ? GetLockOnTarget(PlayerCharacter) : AutoTargetingComp->GetCurrentTargetingActor();
	
	if (!Target) return;

	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const FVector TargetLocation = Target->GetActorLocation();
	float DistanceToTarget = FVector::Dist(PlayerLocation, TargetLocation);
	if (DistanceToTarget > AutoTargetingComp->GetTargetingRange())
	{
		bIsTranslation = true;
		return;
	}
	
	if (DistanceToTarget <= AttackApproachData.ReachDistance)
	{
		bIsTranslation = true;
		return;
	}
	
	const FRotator TargetDirection = (Target->GetActorLocation() - PlayerCharacter->GetActorLocation()).Rotation();
	const FVector NewLocation = FMath::VInterpTo(PlayerLocation,
		TargetLocation - TargetDirection.Vector() * AttackApproachData.ReachDistance,
		FrameDeltaTime, AttackApproachData.ApproachSpeed);
	
	PlayerCharacter->SetActorLocation(NewLocation);
}

AActor* UANS_TranslationDistance_Player::GetLockOnTarget(const ATwoMinPlayerCharacter* PlayerCharacter)
{
	UTwoMinGameplayAbility* Ability =
		PlayerCharacter->GetAbilitySystemComponent()->GetPlayingAbilityTag(TwoMinGameplayTag::Player_Ability_LockOn);
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
