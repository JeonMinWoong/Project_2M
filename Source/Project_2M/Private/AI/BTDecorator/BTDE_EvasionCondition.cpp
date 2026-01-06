// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_EvasionCondition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinBaseCharacter.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

bool UBTDE_EvasionCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = GetBlackboardComponent(OwnerComp);
	if (!BB) return false;

	UObject* Object = BB->GetValueAsObject(TwoMinBBKeys::BattleTarget);
	if (!Object) return false;

	ATwoMinBaseCharacter* BattleTarget = Cast<ATwoMinBaseCharacter>(Object);
	if (!BattleTarget) return false;

	UEnemyCombatComponent* EnemyCombatComponent = GetEnemyCombatComponent(OwnerComp);
	if (!EnemyCombatComponent) return false;
	return EnemyCombatComponent->IsEvasionCondition(BattleTarget, EvasionConditionType);
}
