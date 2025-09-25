// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_DetectCondition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTDecorator_DetectCondition::UBTDecorator_DetectCondition()
{
	NodeName = "DetectCondition";
}

bool UBTDecorator_DetectCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return false;
	
	UObject* TargetObj = BB->GetValueAsObject(BBKeys::TargetActor);
	if (!TargetObj) return false;

	const float DistToTarget = BB->GetValueAsFloat(BBKeys::DistToTarget);
	return DetectRadius >= DistToTarget;
}
