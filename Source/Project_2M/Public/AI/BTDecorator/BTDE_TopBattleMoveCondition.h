// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTDecorator/BTDecorator_Enemy.h"
#include "BTDE_TopBattleMoveCondition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTDE_TopBattleMoveCondition : public UBTDecorator_Enemy
{
	GENERATED_BODY()

public:
	UBTDE_TopBattleMoveCondition();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
