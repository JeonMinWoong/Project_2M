// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTDecorator/BTDecorator_Enemy.h"
#include "BTDE_AttackCondition.generated.h"

class ATwoMinEnemyCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTDE_AttackCondition : public UBTDecorator_Enemy
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

private:
	bool CheckAttackRange(const AActor* MyActor, const AActor* TargetActor) const;
	bool CheckTargetAngle(const AActor* MyActor, const AActor* TargetActor) const;
	bool CheckAbilityCooldown(AActor* MyActor) const;

	bool IsAllCondition(const AActor* MyActor, const AActor* TargetActor) const;
	
	UPROPERTY(EditAnywhere, Category = "AttackRange", meta = (ClampMin = "0.0"))
	float MinAttackRange;

	UPROPERTY(EditAnywhere, Category = "AttackRange", meta = (ClampMin = "0.0"))
	float MaxAttackRange;

	UPROPERTY(EditAnywhere, Category = "TargetAngle", meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float TargetAngle;
	
	UPROPERTY(EditAnywhere, Category = "AbilityCooldown")
	FGameplayTag AbilityCooldownTag;
};
