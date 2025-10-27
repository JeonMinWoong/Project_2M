// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTService/BTService_Enemy.h"
#include "BTSE_LookDirection.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTSE_LookDirection : public UBTService_Enemy
{
	GENERATED_BODY()

public:
	UBTSE_LookDirection();
	
protected:
	//~ Begin UBTService Interface.
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	//~ End UBTService Interface.

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "RotationSpeed")
	float RotationSpeed;
	
};
