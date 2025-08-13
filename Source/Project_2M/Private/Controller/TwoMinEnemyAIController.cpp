// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/TwoMinEnemyAIController.h"

ATwoMinEnemyAIController::ATwoMinEnemyAIController()
{
	AAIController::SetGenericTeamId(FGenericTeamId(1));
}

void ATwoMinEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}
