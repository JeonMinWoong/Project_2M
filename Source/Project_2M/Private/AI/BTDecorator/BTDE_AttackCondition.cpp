// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_AttackCondition.h"

#include "TwoMinFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

bool UBTDE_AttackCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = GetBlackboardComponent(OwnerComp);
	if (!BB) return false;

	UObject* Object = BB->GetValueAsObject(TwoMinBBKeys::BattleTarget);
	if (!Object) return false;

	ATwoMinBaseCharacter* BattleTarget = Cast<ATwoMinBaseCharacter>(Object);
	if (!BattleTarget) return false;

	UEnemyCombatComponent* EnemyCombatComponent = GetEnemyCombatComponent(OwnerComp);
	if (!EnemyCombatComponent) return false;
	
	if (EnemyCombatComponent->IsBanAttack()) return false;
	
	return EnemyCombatComponent->IsAttackCondition(BattleTarget, AttackConditionIndex);
}