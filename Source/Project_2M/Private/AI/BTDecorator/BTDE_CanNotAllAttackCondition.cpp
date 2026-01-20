// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_CanNotAllAttackCondition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTDE_CanNotAllAttackCondition::UBTDE_CanNotAllAttackCondition()
{
	NodeName = "Can Not All Attack Condition";
}

bool UBTDE_CanNotAllAttackCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	UEnemyCombatComponent* CombatComponent = GetEnemyCombatComponent(OwnerComp);
	if (!CombatComponent) return false;

	return CombatComponent->IsEvenOneAttackCooldown() == false || CombatComponent->IsBanAttack();
}
