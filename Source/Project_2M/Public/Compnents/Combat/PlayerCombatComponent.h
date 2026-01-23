// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/ExecutionComponent.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class UAutoTargetingComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UPlayerCombatComponent : public UBaseCombatComponent
{
	GENERATED_BODY()

public:
	UPlayerCombatComponent();

	virtual void OnHitTargetActor(AActor* HitActor, int32 HitWeaponIndex) override;
	virtual UAutoTargetingComponent* GetAutoTargetingComponent() const override;
	virtual UExecutionComponent* GetExecutionComponent() const override;

private:
	UPROPERTY(EditAnywhere)
	UAutoTargetingComponent* AutoTargetComponent;
	
	UPROPERTY(EditAnywhere)
	UExecutionComponent* ExecutionComponent;
};
