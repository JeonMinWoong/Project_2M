// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_PatrolCondition.h"

#include "AIController.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/EnemyCombatComponent.h"

UBTDE_PatrolCondition::UBTDE_PatrolCondition()
{
	NodeName = "PatrolCondition";
}

bool UBTDE_PatrolCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return false;

	const ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return false;

	const UEnemyCombatComponent* EnemyCombatComponent =
		Cast<UEnemyCombatComponent>(EnemyCharacter->GetCombatComponent());
	if (!EnemyCombatComponent) return false;
	
	return EnemyCombatComponent->PatrolPoints.IsEmpty() == false;
}
