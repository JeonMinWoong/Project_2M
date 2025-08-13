// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TwoMinEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATwoMinEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI|BehaviorTree")
	UBehaviorTree* BehaviorTree;
	
};
