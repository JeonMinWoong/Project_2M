// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTE_SelectPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTTE_SelectPatrolPoint::UBTTE_SelectPatrolPoint()
{
	NodeName = "SelectPatrolPoint";
}

EBTNodeResult::Type UBTTE_SelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;

	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;
	
	UEnemyCombatComponent* EnemyCombatComponent =
		Cast<UEnemyCombatComponent>(EnemyCharacter->GetCombatComponent());
	if (EnemyCombatComponent->IsPatrol())
	{
		int32 CurrentPoint = EnemyCombatComponent->GetCurrentPatrolPointIndex();
		float Dist = FVector::Dist(EnemyCharacter->GetActorLocation(),
			EnemyCombatComponent->PatrolPoints[CurrentPoint].PatrolPoint->GetActorLocation());
		if (Dist <= PatrolPointReachDistance)
		{
			EnemyCombatComponent->ResetPatrolPoint();
		}
		
		return EBTNodeResult::Failed;
	}
	
	int32 NextPoint = EnemyCombatComponent->GetNextPatrolPointIndex();
	BB->SetValueAsVector(TwoMinBBKeys::PatrolPoint,
		EnemyCombatComponent->PatrolPoints[NextPoint].PatrolPoint->GetActorLocation());
	return EBTNodeResult::Succeeded;
}


