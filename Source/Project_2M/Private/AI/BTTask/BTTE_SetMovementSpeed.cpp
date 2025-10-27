// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTE_SetMovementSpeed.h"

#include "AIController.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTE_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyCharacter) return EBTNodeResult::Failed;
	
	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = NewMovementSpeed;
	return EBTNodeResult::Succeeded;
}
