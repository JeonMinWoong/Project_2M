// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/TwoMinBaseAnimInstance.h"
#include "TwoMinCharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class ATwoMinBaseCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinCharacterAnimInstance : public UTwoMinBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY()
	ATwoMinBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimData|Locomotion")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimData|Locomotion")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimData|Locomotion")
	float LocomotionDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimData|Locomotion")
	bool bIsPossibleBreaker;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimData|Locomotion")
	bool bShouldEnterBreakerState;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimData|Locomotion")
	float EnterBreakerStateThreshold = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AnimData|Locomotion")
	float EnterIdleStateThreshold = 5.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimData|Locomotion")
	bool bIsBattlePossible;
	
	float IdleElapsedTime;
	float IdleBreakerElapsedTime;

private:
	void UpdateBattlePossible();
	void UpdateIdleElapsedTime(float DeltaSeconds);
	
};
