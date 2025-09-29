// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_BattleCondition.h"

UBTDE_BattleCondition::UBTDE_BattleCondition()
{
	NodeName = "BattleCondition";
}

bool UBTDE_BattleCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return GetEnemyCombatComponent(OwnerComp)->IsBattlePossible();
}
