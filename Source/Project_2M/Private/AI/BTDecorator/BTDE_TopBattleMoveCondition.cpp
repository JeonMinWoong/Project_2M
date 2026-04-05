// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_TopBattleMoveCondition.h"

#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTDE_TopBattleMoveCondition::UBTDE_TopBattleMoveCondition()
{
	NodeName = "Top Battle Move Condition";
}

bool UBTDE_TopBattleMoveCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UEnemyCombatComponent* EnemyCombatComp = GetEnemyCombatComponent(OwnerComp);
	if (!EnemyCombatComp) return false;
	
	if (UTwoMinFunctionLibrary::HasGameplayTag(EnemyCombatComp->GetOwner(), TwoMinGameplayTag::Enemy_State_Actioning)) 
		return false;;
	
	UBlackboardComponent* BB = GetBlackboardComponent(OwnerComp);
	if (!BB) return false;

	FVector BattleMovePointVector = BB->GetValueAsVector(TwoMinBBKeys::BattleMovePoint);
	return BattleMovePointVector.IsNearlyZero();
}