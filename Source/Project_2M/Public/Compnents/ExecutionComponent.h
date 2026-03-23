// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Compnents/BaseComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "ExecutionComponent.generated.h"

class ATwoMinEnemyCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UExecutionComponent : public UBaseComponent
{
	GENERATED_BODY()

public:
	UExecutionComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
		FActorComponentTickFunction *ThisTickFunction) override;
	
private:
	void UpdateGroggyEnemies();

	ATwoMinBaseCharacter* IsExecutionCondition(ATwoMinBaseCharacter* MyCaster);
	int GetScoreCalculation(float CurrentValue, int MaxScore, int MinValue) const;

	UPROPERTY()
	TArray<ATwoMinBaseCharacter*> GroggyEnemies;

	UPROPERTY()
	ATwoMinBaseCharacter* ExecutionTarget;
	
	UPROPERTY(EditAnywhere)
	FExecutionData ExecutionData;

	UPROPERTY()
	bool bIsExecutionForward = false;

	UPROPERTY()
	int32 ExecutionNumber;
	
	UPROPERTY()
	bool bIsPlayingExecution = false;
	
public:
	FORCEINLINE ATwoMinBaseCharacter* GetExecutionTarget() const { return ExecutionTarget; }
	FORCEINLINE bool IsExecutionForward() const { return bIsExecutionForward; }
	FORCEINLINE void SetExecutionNumber(int32 Value) { ExecutionNumber = Value; }
	FORCEINLINE int32 GetExecutionNumber() const { return ExecutionNumber; }
	FORCEINLINE void SetIsPlayingExecution(bool Value) { bIsPlayingExecution = Value; }
	
};
