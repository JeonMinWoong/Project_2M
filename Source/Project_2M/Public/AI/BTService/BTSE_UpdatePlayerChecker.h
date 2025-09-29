// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTService/BTService_Enemy.h"
#include "BTSE_UpdatePlayerChecker.generated.h"

class ATwoMinBaseCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTSE_UpdatePlayerChecker : public UBTService_Enemy
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	ATwoMinBaseCharacter* UpdatePlayerChecker(AActor* EnemyCharacter) const;

	UPROPERTY(EditAnywhere, Category = "CheckRange")
	float CheckRange;
	
};
