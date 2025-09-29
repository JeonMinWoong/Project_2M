// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_StopAllLogicCondition.h"

bool UBTDE_StopAllLogicCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UEnemyCombatComponent* EnemyCombatComponent = GetEnemyCombatComponent(OwnerComp);
	if (!EnemyCombatComponent) return false;
	return EnemyCombatComponent->GetIsAlive() == false;
}
