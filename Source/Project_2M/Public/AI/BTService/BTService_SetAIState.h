// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SetAIState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTService_SetAIState : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "OutTarget")
	FBlackboardKeySelector OutTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AIState")
	FString AIStateString;
	
};
