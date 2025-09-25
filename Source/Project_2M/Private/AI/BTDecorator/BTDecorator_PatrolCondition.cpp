// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_PatrolCondition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTDecorator_PatrolCondition::UBTDecorator_PatrolCondition()
{
	NodeName = "PatrolCondition";
}

bool UBTDecorator_PatrolCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	return PatrolPoints.IsEmpty() == false;
}
