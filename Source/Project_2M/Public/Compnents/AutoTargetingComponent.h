// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compnents/BaseComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "AutoTargetingComponent.generated.h"

class ATwoMinPlayerCharacter;
class ATwoMinEnemyCharacter;
struct FTwoMinPlayerAutoTargetingData;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UAutoTargetingComponent : public UBaseComponent
{
	GENERATED_BODY()

public:
	void StartAutoTargeting();
	void EndAutoTargeting();
	
private:
	ATwoMinEnemyCharacter* IsTargetingCondition(ATwoMinPlayerCharacter* PlayerCharacter, TArray<AActor*> Actors,
		const FVector& InputForward) const;

	void DrawDebug(const FVector& StartLocation, const FVector& InputForward) const;
	
	UPROPERTY()
	AActor* TargetingActor;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bIsDebugTargetingRange;
	
	UPROPERTY()
	ATwoMinEnemyCharacter* CurrentTargetingActor;

	UPROPERTY(EditAnywhere)
	FTwoMinPlayerAutoTargetingData TargetingData;

public:
	FORCEINLINE ATwoMinEnemyCharacter* GetCurrentTargetingActor() const { return CurrentTargetingActor; }
	FORCEINLINE float GetTargetingRotationSpeed() const { return TargetingData.TargetingRotationSpeed; }
};
