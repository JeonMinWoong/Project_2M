// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/TwoMinCharacterAnimInstance.h"
#include "TwoMinPlayerAnimInstance.generated.h"

class ATwoMinPlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinPlayerAnimInstance : public UTwoMinCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="AnimData|Refrencs")
	ATwoMinPlayerCharacter* OwningPlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AnimData|Locomotion")
	bool bIsRun = false;
};
