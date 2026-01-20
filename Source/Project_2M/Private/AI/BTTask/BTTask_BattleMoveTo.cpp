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

EBTNodeResult::Type UBTTask_BattleMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
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
	
	bool bIsStopBattleMove =
		EnemyCombatComponent->IsStopBattleMoveGameplayContainer() ||
			(EnemyCombatComponent->IsBanAttack() == false && EnemyCombatComponent->IsEvenOneAttackCondition(TargetActor));
	if (bIsStopBattleMove == false) return;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}

void UBTTask_BattleMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	if (bIsCheckArrive == false) return;
	
	if (TaskResult != EBTNodeResult::Succeeded) return;
	
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return;
	
	UBlackboardComponent* BB = AI->GetBlackboardComponent();
	if (!BB) return;	
	
	BB->SetValueAsBool(TwoMinBBKeys::bIsArriveBattleMovePoint, true);
}
