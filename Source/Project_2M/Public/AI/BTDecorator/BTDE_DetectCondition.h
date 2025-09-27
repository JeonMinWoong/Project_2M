// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTDecorator_Enemy.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDE_DetectCondition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTDE_DetectCondition : public UBTDecorator_Enemy
{
	GENERATED_BODY()

public:
	UBTDE_DetectCondition();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "DetectRadius")
	float DetectRadius;
};
