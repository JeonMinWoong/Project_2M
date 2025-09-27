// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_CanPatrolMoveTo.h"

#include "AIController.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/EnemyCombatComponent.h"

UBTDE_CanPatrolMoveTo::UBTDE_CanPatrolMoveTo()
{
	NodeName = TEXT("Can Patrol Move To");
}

bool UBTDE_CanPatrolMoveTo::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                              uint8* NodeMemory) const
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return false;

	const ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return false;

	UEnemyCombatComponent* EnemyCombatComponent =
		Cast<UEnemyCombatComponent>(EnemyCharacter->GetCombatComponent());
	if (!EnemyCombatComponent) return false;

	if (EnemyCombatComponent->IsPatrol())
	{
		return false;
	}

	EnemyCombatComponent->SetIsPatrol(true);
	return true;
}
