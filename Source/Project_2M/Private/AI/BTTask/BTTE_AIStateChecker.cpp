// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTE_AIStateChecker.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/UI/EnemyUIComponent.h"

UBTTE_AIStateChecker::UBTTE_AIStateChecker()
{
	NodeName = TEXT("AI State Checker");
}

EBTNodeResult::Type UBTTE_AIStateChecker::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;

	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return EBTNodeResult::Failed;

	UEnemyUIComponent* EnemyUIComponent = EnemyCharacter->GetEnemyUIComponent();
	if (!EnemyUIComponent) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;
	
	EnemyUIComponent->OnEnemyStateChanged.Broadcast(BB->GetValueAsString(FName("AIState")));
	
	return EBTNodeResult::Succeeded;
}


