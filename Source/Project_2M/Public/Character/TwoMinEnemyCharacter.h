// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TwoMinBaseCharacter.h"
#include "TwoMinEnemyCharacter.generated.h"

class UWidgetComponent;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinEnemyCharacter : public ATwoMinBaseCharacter
{
	GENERATED_BODY()

public:
	ATwoMinEnemyCharacter();
	
	virtual UBaseCombatComponent* GetCombatComponent() const override;
	virtual UBaseUIComponent* GetBaseUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;

	FTimerHandle DecreaseGroggyTimerHandle;
	
protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UEnemyUIComponent* EnemyUIComponent;
	
private:
	/** Components **/
	UPROPERTY(VisibleAnywhere)
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere, Category= "UI")
	UWidgetComponent* EnemyHealthWidgetComponent;
	
	/** CharacterInfo **/
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|CharacterMovement")
	float MaxWalkSpeed = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|CharacterMovement")
	FRotator CharacterRotationRate = FRotator(0.f, 500.f, 0.f);
};
