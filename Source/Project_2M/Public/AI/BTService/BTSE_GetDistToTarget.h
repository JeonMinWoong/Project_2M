// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTService_Enemy.h"
#include "BehaviorTree/BTService.h"
#include "BTSE_GetDistToTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTSE_GetDistToTarget : public UBTService_Enemy
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "OutTarget")
	FBlackboardKeySelector OutTargetActorKey;
};
