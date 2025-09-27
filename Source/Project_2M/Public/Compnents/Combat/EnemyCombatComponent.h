// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Engine/TargetPoint.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "EnemyCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UEnemyCombatComponent : public UBaseCombatComponent
{
	GENERATED_BODY()

public:
	UEnemyCombatComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
								   FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void OnHitTargetActor(AActor* HitActor) override;

	int32 GetNextPatrolPointIndex();

	void ResetPatrolPoint();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPoints")
	TArray<FEnemyAIPatrolData> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolMode")
	EPatrolPathMode PatrolPathMode = EPatrolPathMode::Loop;
	
private:
	UFUNCTION()
	void PatrolWaitPoint();
	
	UPROPERTY(VisibleAnywhere, Category = "IsPatrol")
	bool bIsPatrol;

	UPROPERTY(VisibleAnywhere, Category = "CurrentPatrolPoint")
	int32 CurrentPatrolPointIndex = 0;

	bool bPingPongForward = true;
	bool bIsPatrolPointArrivedWait = false;
	float CurrentPatrolPointArrivedWaitTime = 0.f;

	
public:
	FORCEINLINE bool IsPatrol() const { return bIsPatrol; }
	FORCEINLINE void SetIsPatrol(bool InIsPatrol) { bIsPatrol = InIsPatrol; }
	FORCEINLINE int32 GetCurrentPatrolPointIndex() const { return CurrentPatrolPointIndex; }
};
