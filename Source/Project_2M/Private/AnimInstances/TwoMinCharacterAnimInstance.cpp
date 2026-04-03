// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/TwoMinCharacterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTwoMinCharacterAnimInstance::NativeInitializeAnimation()
{
	OwnerCharacter = Cast<ATwoMinBaseCharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UTwoMinCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwnerCharacter || !OwnerMovementComponent) return;

	GroundSpeed = OwnerCharacter->GetVelocity().Size2D();
	bHasAcceleration = OwnerMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwnerCharacter->GetVelocity(),
		OwnerCharacter->GetActorRotation());
	
	UpdateBattlePossible();
	UpdateIdleElapsedTime(DeltaSeconds);
}

void UTwoMinCharacterAnimInstance::UpdateBattlePossible()
{
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(OwnerCharacter);
	if (!EnemyCharacter) return;
	UEnemyCombatComponent* EnemyCombatComponent = Cast<UEnemyCombatComponent>(EnemyCharacter->GetCombatComponent());
	if (!EnemyCombatComponent) return;
	
	bIsBattlePossible = EnemyCombatComponent->IsBattlePossible();
}

void UTwoMinCharacterAnimInstance::UpdateIdleElapsedTime(float DeltaSeconds)
{
	if (bIsPossibleBreaker == false) return;
	
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(OwnerCharacter);
	if (!EnemyCharacter) return;
	
	if (bIsBattlePossible)
	{
		IdleElapsedTime = 0.f;
		IdleBreakerElapsedTime = 0.f;
		bShouldEnterBreakerState = false;
		return;
	}
	
	if (GroundSpeed > 0)
	{
		IdleElapsedTime = 0.f;
		IdleBreakerElapsedTime = 0.f;
		bShouldEnterBreakerState = false;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterBreakerState = (IdleElapsedTime >= EnterBreakerStateThreshold);
		if (bShouldEnterBreakerState)
		{
			IdleBreakerElapsedTime += DeltaSeconds;
			if (IdleBreakerElapsedTime >= EnterIdleStateThreshold)
			{
				IdleElapsedTime = 0.f;
				IdleBreakerElapsedTime = 0.f;
				bShouldEnterBreakerState = false;
			}
		}
	}
}
