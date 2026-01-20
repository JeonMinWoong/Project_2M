// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_CompareMoveType.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTDE_CompareMoveType::UBTDE_CompareMoveType()
{
	NodeName = "Compare Move Type";
}

bool UBTDE_CompareMoveType::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = GetBlackboardComponent(OwnerComp);
	if (!BB) return false;
	
	EEnemyAIMoveType MoveType = static_cast<EEnemyAIMoveType>(BB->GetValueAsEnum(TwoMinBBKeys::SelectMoveType));
	return MoveTypeToCompare == MoveType;
}
