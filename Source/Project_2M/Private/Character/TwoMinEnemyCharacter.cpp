// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TwoMinEnemyCharacter.h"

#include "Compnents/Combat/EnemyCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATwoMinEnemyCharacter::ATwoMinEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = CharacterRotationRate;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f; // 감속 속도

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");
}

UBaseCombatComponent* ATwoMinEnemyCharacter::GetCombatComponent() const
{
	return EnemyCombatComponent;
}