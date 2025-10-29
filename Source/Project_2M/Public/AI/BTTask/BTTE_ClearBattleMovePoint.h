// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask/BTTask_Enemy.h"
#include "BTTE_ClearBattleMovePoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTTE_ClearBattleMovePoint : public UBTTask_Enemy
{
	GENERATED_BODY()

public:
	UBTTE_ClearBattleMovePoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
