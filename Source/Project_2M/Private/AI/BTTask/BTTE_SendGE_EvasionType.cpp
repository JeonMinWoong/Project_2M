// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTE_SendGE_EvasionType.h"

#include "AIController.h"
#include "TwoMinFunctionLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Character/TwoMinEnemyCharacter.h"

EBTNodeResult::Type UBTTE_SendGE_EvasionType::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;

	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return EBTNodeResult::Failed;
	
	FGameplayEventData EventData;
	EventData.Instigator = EnemyCharacter;
	EventData.EventMagnitude = static_cast<float>(EvasionType);
	
	UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
		EnemyCharacter, 
		EvasionEventTag, 
		EventData
	);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
