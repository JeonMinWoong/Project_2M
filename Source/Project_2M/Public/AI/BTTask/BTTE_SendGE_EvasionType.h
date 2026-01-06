// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TwoMinGameplayTag.h"
#include "AI/BTTask/BTTask_Enemy.h"
#include "BTTE_SendGE_EvasionType.generated.h"

enum class EEnemyEvasionType : uint8;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBTTE_SendGE_EvasionType : public UBTTask_Enemy
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY()
	FGameplayTag EvasionEventTag = TwoMinGameplayTag::Enemy_Event_Evasion;
	
	UPROPERTY(EditAnywhere, Category = "EvasionType")
	EEnemyEvasionType EvasionType;
};


