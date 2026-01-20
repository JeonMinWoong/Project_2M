// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_BattleMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTTask_BattleMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UBTTask_BattleMoveTo();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "CheckArrive")
	bool bIsCheckArrive = false;
};
