// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_BattleActionCondition.h"

#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"

UBTDE_BattleActionCondition::UBTDE_BattleActionCondition()
{
	NodeName = "BattleActionCondition";
}

bool UBTDE_BattleActionCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	
	UEnemyCombatComponent* EnemyCombatComp = GetEnemyCombatComponent(OwnerComp);
	if (!EnemyCombatComp) return false;
	
	return !UTwoMinFunctionLibrary::HasGameplayTag(EnemyCombatComp->GetOwner(), TwoMinGameplayTag::Enemy_State_Actioning);
}
