// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_Enemy.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTE_SelectPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTTE_SelectPatrolPoint : public UBTTask_Enemy
{
	GENERATED_BODY()

public:
	UBTTE_SelectPatrolPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "PatrolDist")
	float PatrolPointReachDistance = 100.f;
	
};
