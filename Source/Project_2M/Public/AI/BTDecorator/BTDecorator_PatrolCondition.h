// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_PatrolCondition.generated.h"

class ATargetPoint;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTDecorator_PatrolCondition : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_PatrolCondition();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "PatrolPoints")
	TArray<ATargetPoint*> PatrolPoints;
};
