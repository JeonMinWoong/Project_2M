// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_Enemy.h"

#include "AIController.h"
#include "Character/TwoMinEnemyCharacter.h"

UBlackboardComponent* UBTService_Enemy::GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return nullptr;

	return BB;
}

ATwoMinEnemyCharacter* UBTService_Enemy::GetEnemyCharacter(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return nullptr;

	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return nullptr;
	
	return EnemyCharacter;
}

UEnemyCombatComponent* UBTService_Enemy::GetEnemyCombatComponent(UBehaviorTreeComponent& OwnerComp) const
{
	ATwoMinEnemyCharacter* EnemyCharacter = GetEnemyCharacter(OwnerComp);
	if (!EnemyCharacter) return nullptr;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return nullptr;
	
	UEnemyCombatComponent* EnemyCombatComponent =
		Cast<UEnemyCombatComponent>(EnemyCharacter->GetCombatComponent());

	return EnemyCombatComponent;
}
