// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "BTService_Enemy.generated.h"

class ATwoMinEnemyCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTService_Enemy : public UBTService
{
	GENERATED_BODY()

protected:
	UBlackboardComponent* GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp) const;
	ATwoMinEnemyCharacter* GetEnemyCharacter(UBehaviorTreeComponent& OwnerComp) const;
	UEnemyCombatComponent* GetEnemyCombatComponent(UBehaviorTreeComponent& OwnerComp) const;
};
