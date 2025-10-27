// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Enemy.generated.h"

class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTTask_Enemy : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UEnemyCombatComponent* GetEnemyCombatComponent(UBehaviorTreeComponent& OwnerComp) const;
};
