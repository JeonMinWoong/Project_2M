// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_DetectCondition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTDE_DetectCondition::UBTDE_DetectCondition()
{
	NodeName = "DetectCondition";
}

bool UBTDE_DetectCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return false;
	
	UObject* TargetObj = BB->GetValueAsObject(TwoMinBBKeys::TargetActor);
	if (!TargetObj) return false;

	const float DistToTarget = BB->GetValueAsFloat(TwoMinBBKeys::DistToTarget);
	
	ATwoMinEnemyCharacter* EnemyCharacter = GetEnemyCharacter(OwnerComp);
	if (!EnemyCharacter) return false;
	
	UEnemyCombatComponent* CombatComponent = GetEnemyCombatComponent(OwnerComp);
	if (!CombatComponent) return false;
	
	float BaseDetectRange = DetectRange;
	const float CustomDetectRange = EnemyCharacter->IsUseBossHealthBar() ? CombatComponent->GetCustomDetectRange() : 0;
	if (CustomDetectRange > 0.f)
	{
		BaseDetectRange = CustomDetectRange;
	}
	
	return BaseDetectRange >= DistToTarget && CombatComponent->IsBattlePossible() == false && CombatComponent->IsEquip() == false;
}