// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask/BTTask_Enemy.h"
#include "BTTE_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTTE_SetMovementSpeed : public UBTTask_Enemy
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "MovementSpeed")
	float NewMovementSpeed;
	
};
