// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTE_CustomRunEQSQuery.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

EBTNodeResult::Type UBTTE_CustomRunEQSQuery::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!EQSQuery) return EBTNodeResult::Failed;
	if (BlackboardKey.IsNone()) return EBTNodeResult::Failed;

	RunEQSQuery(OwnerComp);

	return EBTNodeResult::InProgress;
}

void UBTTE_CustomRunEQSQuery::RunEQSQuery(UBehaviorTreeComponent& OwnerComp)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return;

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		AI,
		EQSQuery,
		AI->GetPawn(),
		EEnvQueryRunMode::AllMatching,
		nullptr
	);

	if (!QueryInstance) return;
	
	MyOwnerComp = &OwnerComp;
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &UBTTE_CustomRunEQSQuery::OnQueryFinished);
}

void UBTTE_CustomRunEQSQuery::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                              EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		FinishLatentTask(*MyOwnerComp, EBTNodeResult::Failed);
		return;
	}

	int32 TotalNum = QueryInstance->GetResultsAsLocations().Num() - 1;
	int32 MinNum = FMath::CeilToInt(TotalNum * MinRate);
	
	int32 RandomIndex = FMath::RandRange(0, MinNum);
	FVector ResultLocation = QueryInstance->GetResultsAsLocations()[RandomIndex];

	MyOwnerComp->GetBlackboardComponent()->SetValueAsVector(TwoMinBBKeys::BattleMovePoint, ResultLocation);

	FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
}
