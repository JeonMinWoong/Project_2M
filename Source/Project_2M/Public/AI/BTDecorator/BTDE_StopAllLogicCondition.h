// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTDecorator/BTDecorator_Enemy.h"
#include "BTDE_StopAllLogicCondition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTDE_StopAllLogicCondition : public UBTDecorator_Enemy
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
