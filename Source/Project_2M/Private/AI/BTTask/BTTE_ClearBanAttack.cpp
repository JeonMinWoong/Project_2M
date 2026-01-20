// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTE_ClearBanAttack.h"

#include "Compnents/Combat/EnemyCombatComponent.h"

EBTNodeResult::Type UBTTE_ClearBanAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UEnemyCombatComponent* EnemyCombatComponent = GetEnemyCombatComponent(OwnerComp);
	if (!EnemyCombatComponent) return EBTNodeResult::Failed;
	
	EnemyCombatComponent->ClearBanAttack();
	return EBTNodeResult::Succeeded;
}
