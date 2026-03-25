// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TwoMinPlayerMovementComp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinPlayerMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	virtual FVector ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const override;
	
};
