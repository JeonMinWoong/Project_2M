// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTDecorator_Enemy.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDE_PatrolCondition.generated.h"

class ATargetPoint;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTDE_PatrolCondition : public UBTDecorator_Enemy
{
	GENERATED_BODY()

public:
	UBTDE_PatrolCondition();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
