// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTDecorator/BTDecorator_Enemy.h"
#include "BTDE_BattleMoveCondition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTDE_BattleMoveCondition : public UBTDecorator_Enemy
{
	GENERATED_BODY()

public:
	UBTDE_BattleMoveCondition();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	bool CheckAttackRange(const AActor* MyActor, const AActor* TargetActor) const;
	bool CheckTargetAngle(const AActor* MyActor, const AActor* TargetActor) const;
	
	UPROPERTY(EditAnywhere, Category = "AttackRange", meta = (ClampMin = "0.0"))
	float MinAttackRange;

	UPROPERTY(EditAnywhere, Category = "OutAttackRange", meta = (ClampMin = "0.0"))
	float MaxAttackRange;

	UPROPERTY(EditAnywhere, Category = "TargetAngle", meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float TargetAngle;
};
