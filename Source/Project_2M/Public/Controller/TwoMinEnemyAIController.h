// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "TwoMinEnemyAIController.generated.h"

class UAISenseConfig_Sight;

/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATwoMinEnemyAIController(const FObjectInitializer& ObjectInitializer);
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	UFUNCTION()
	void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(EditDefaultsOnly, Category = "AI|BehaviorTree")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	UPROPERTY(EditAnywhere)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config")
	float CollisionQueryRange = 150.f;
	
};
