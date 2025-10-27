// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask/BTTask_Enemy.h"
#include "BTTE_AttackMoveWait.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTTE_AttackMoveWait : public UBTTask_Enemy
{
	GENERATED_BODY()

public:
	UBTTE_AttackMoveWait();
	
protected:
	virtual FString GetStaticDescription() const override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CanAttackMoveWait(UBehaviorTreeComponent& OwnerComp) const;
	
	UPROPERTY(EditAnywhere, Category = "AttackMoveWait", meta = (ClampMin = 0))
	float MinRange;

	UPROPERTY(EditAnywhere, Category = "AttackMoveWait", meta = (ClampMin = 0))
	float MaxRange;
	
	UPROPERTY(EditAnywhere, Category = "AttackMoveWait")
	FGameplayTagContainer CheckAttackCooldownTags;

	float TotalWaitTime;
	float ElapsedTime;
};
