// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIStateChecker.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTTask_AIStateChecker : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AIStateChecker();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
