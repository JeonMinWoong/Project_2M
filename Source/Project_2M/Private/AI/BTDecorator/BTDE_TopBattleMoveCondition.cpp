// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_TopBattleMoveCondition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTDE_TopBattleMoveCondition::UBTDE_TopBattleMoveCondition()
{
	NodeName = "Top Battle Move Condition";
}

bool UBTDE_TopBattleMoveCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = GetBlackboardComponent(OwnerComp);
	if (!BB) return false;

	FVector BattleMovePointVector = BB->GetValueAsVector(TwoMinBBKeys::BattleMovePoint);
	return BattleMovePointVector.IsNearlyZero();
}