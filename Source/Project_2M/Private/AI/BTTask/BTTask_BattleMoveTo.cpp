// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_BattleMoveTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTTask_BattleMoveTo::UBTTask_BattleMoveTo()
{
	bNotifyTick = true;
}

void UBTTask_BattleMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return;

	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return;
	
	UEnemyCombatComponent* EnemyCombatComponent = Cast<UEnemyCombatComponent>(EnemyCharacter->GetCombatComponent());
	if (!EnemyCombatComponent) return;

	UBlackboardComponent* BB = AI->GetBlackboardComponent();
	if (!BB) return;
	
	UObject* Object = BB->GetValueAsObject(TwoMinBBKeys::BattleTarget);
	if (!Object) return;

	AActor* TargetActor = Cast<AActor>(Object);
	if (!TargetActor) return;
	
	if (EnemyCombatComponent->IsEvenOneAttackCondition(TargetActor) == false) return;

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
