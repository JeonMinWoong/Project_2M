// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask/BTTask_Enemy.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"
#include "BTTE_CustomRunEQSQuery.generated.h"

class UEnvQuery;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTTE_CustomRunEQSQuery : public UBTTask_Enemy
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void RunEQSQuery(UBehaviorTreeComponent& OwnerComp);
	
	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UPROPERTY(EditAnywhere, Category = "EQS")
	UEnvQuery* EQSQuery;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BlackboardKey;
	
	// 높을 수록 점수가 낮은 위치도 포함.
	UPROPERTY(EditAnywhere, Category = "MinRate", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MinRate = 1.f;
	
	UPROPERTY()
	FName ArriveName = TwoMinBBKeys::bIsArriveBattleMovePoint;
	
	UPROPERTY()
	UBehaviorTreeComponent* MyOwnerComp;
};
