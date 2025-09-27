// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_TargetHasGameplayTag.h"

#include "AIController.h"
#include "TwoMinFunctionLibrary.h"
#include "Character/TwoMinEnemyCharacter.h"

UBTDE_TargetHasGameplayTag::UBTDE_TargetHasGameplayTag()
{
	NodeName = TEXT("TargetHasGameplayTag");
}

bool UBTDE_TargetHasGameplayTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                   uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter) return false;
	
	return UTwoMinFunctionLibrary::HasGameplayTag(EnemyCharacter, RequiredTag);
}
