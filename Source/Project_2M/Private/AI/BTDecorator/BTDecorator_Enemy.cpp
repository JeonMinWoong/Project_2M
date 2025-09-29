// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_Enemy.h"

#include "AIController.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/EnemyCombatComponent.h"

UBlackboardComponent* UBTDecorator_Enemy::GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return nullptr;

	return BB;
}

UEnemyCombatComponent* UBTDecorator_Enemy::GetEnemyCombatComponent(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return nullptr;

	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return nullptr;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return nullptr;
	
	UEnemyCombatComponent* EnemyCombatComponent =
		Cast<UEnemyCombatComponent>(EnemyCharacter->GetCombatComponent());

	return EnemyCombatComponent;
}
