// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTE_AddGameplayTag.h"

#include "AIController.h"
#include "TwoMinFunctionLibrary.h"
#include "Character/TwoMinEnemyCharacter.h"

EBTNodeResult::Type UBTTE_AddGameplayTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;

	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return EBTNodeResult::Failed;

	if (bAddGameplayTag)
	{
		UTwoMinFunctionLibrary::AddGameplayTagToActor(EnemyCharacter, AddTag);	
	}
	else
	{
		UTwoMinFunctionLibrary::RemoveGameplayTagToActor(EnemyCharacter, RemoveTag);
	}
	
	return EBTNodeResult::Succeeded;
}
