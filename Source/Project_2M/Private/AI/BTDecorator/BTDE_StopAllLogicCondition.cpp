// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_StopAllLogicCondition.h"

#include "AIController.h"
#include "TwoMinFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinBaseCharacter.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

bool UBTDE_StopAllLogicCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UEnemyCombatComponent* EnemyCombatComponent = GetEnemyCombatComponent(OwnerComp);
	if (!EnemyCombatComponent) return false;

	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return false;
	UBlackboardComponent* BB = AI->GetBlackboardComponent();
	if (!BB) return false;

	if (UObject* Object = BB->GetValueAsObject(TwoMinBBKeys::BattleTarget))
	{
		if (ATwoMinBaseCharacter* BattleTarget = Cast<ATwoMinBaseCharacter>(Object))
		{
			return BattleTarget->GetCombatComponent()->GetIsAlive() == false;	
		}
	}
	
	return EnemyCombatComponent->GetIsAlive() == false;
}
