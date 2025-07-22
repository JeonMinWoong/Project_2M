// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPlayerCombatComponent();

public:
	virtual UAutoTargetingComponent* GetAutoTargetingComponent() const override;
private:
	UPROPERTY(EditAnywhere)
	UAutoTargetingComponent* AutoTargetComponent;
};
