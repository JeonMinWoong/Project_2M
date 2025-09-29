// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_BattleMoveCondition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinBaseCharacter.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTDE_BattleMoveCondition::UBTDE_BattleMoveCondition()
{
	NodeName = "BattleMoveCondition";
}

bool UBTDE_BattleMoveCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = GetBlackboardComponent(OwnerComp);
	if (!BB) return false;

	UObject* Object = BB->GetValueAsObject(TwoMinBBKeys::BattleTarget);
	if (!Object) return false;

	ATwoMinBaseCharacter* BattleTarget = Cast<ATwoMinBaseCharacter>(Object);
	if (!BattleTarget) return false;

	UEnemyCombatComponent* EnemyCombatComponent = GetEnemyCombatComponent(OwnerComp);
	if (!EnemyCombatComponent) return false;
	
	float DistToBattleTarget = FVector::Dist(EnemyCombatComponent->GetOwner()->GetActorLocation(),
		BattleTarget->GetActorLocation());
	return DistToBattleTarget <= MaxAttackRange && DistToBattleTarget > MinAttackRange;
}