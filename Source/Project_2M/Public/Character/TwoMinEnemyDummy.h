// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "TwoMinEnemyDummy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinEnemyDummy : public ATwoMinEnemyCharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "AttackIndex")
	int32 AttackIndex = 1;

	UPROPERTY(EditAnywhere, Category = "AutoAction")
	bool bAutoAction = false;
	
public:
	FORCEINLINE int32 GetAttackIndex() const { return AttackIndex; }
	FORCEINLINE bool GetAutoAction() const { return bAutoAction; }
};
