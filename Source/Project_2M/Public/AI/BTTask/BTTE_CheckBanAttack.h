// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask/BTTask_Enemy.h"
#include "BTTE_CheckBanAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTTE_CheckBanAttack : public UBTTask_Enemy
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "BanDelay")
	float MinBanDelay = 2.f;
	
	UPROPERTY(EditAnywhere, Category = "BanDelay")
	float MaxBanDelay = 4.f;
};
