// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTDecorator_Enemy.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDE_CanPatrolMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTDE_CanPatrolMoveTo : public UBTDecorator_Enemy
{
	GENERATED_BODY()

public:
	UBTDE_CanPatrolMoveTo();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
