// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Enemy.h"

#include "AIController.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/EnemyCombatComponent.h"

EBTNodeResult::Type UBTTask_Enemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}

void UBTTask_Enemy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

UEnemyCombatComponent* UBTTask_Enemy::GetEnemyCombatComponent(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return nullptr;

	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return nullptr;
	
	UEnemyCombatComponent* EnemyCombatComponent =
		Cast<UEnemyCombatComponent>(EnemyCharacter->GetCombatComponent());

	return EnemyCombatComponent;
}
