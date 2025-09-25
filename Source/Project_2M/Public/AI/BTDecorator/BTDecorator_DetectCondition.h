// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_DetectCondition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTDecorator_DetectCondition : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_DetectCondition();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "DetectRadius")
	float DetectRadius;
};
