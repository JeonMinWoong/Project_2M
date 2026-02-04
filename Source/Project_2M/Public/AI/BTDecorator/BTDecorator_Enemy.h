// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "BTDecorator_Enemy.generated.h"

class ATwoMinEnemyCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTDecorator_Enemy : public UBTDecorator
{
	GENERATED_BODY()

protected:
	UBlackboardComponent* GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp) const;
	ATwoMinEnemyCharacter* GetEnemyCharacter(UBehaviorTreeComponent& OwnerComp) const;
	UEnemyCombatComponent* GetEnemyCombatComponent(UBehaviorTreeComponent& OwnerComp) const;
};
