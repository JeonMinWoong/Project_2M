// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTE_ClearBattleMovePoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTTE_ClearBattleMovePoint::UBTTE_ClearBattleMovePoint()
{
	NodeName = "Clear Battle Move Point";
}

EBTNodeResult::Type UBTTE_ClearBattleMovePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	FVector BattleMovePointVector = BB->GetValueAsVector(TwoMinBBKeys::BattleMovePoint);
	if (BattleMovePointVector.IsNearlyZero()) return EBTNodeResult::Failed;
	
	BB->SetValueAsVector(TwoMinBBKeys::BattleMovePoint, FVector::ZeroVector);
	return EBTNodeResult::Succeeded;
}
