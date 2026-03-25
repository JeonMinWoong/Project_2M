// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/TwoMinPlayerMovementComp.h"

#include "InputActionValue.h"
#include "TwoMinConstant.h"
#include "TwoMinDebugHelper.h"
#include "Character/TwoMinPlayerCharacter.h"

FVector UTwoMinPlayerMovementComp::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal,
                                                      const FHitResult& Hit) const
{
	if (Normal.Z >= TwoMinConstant::ComputeBlockValue)
	{
		return Super::ComputeSlideVector(Delta, Time, Normal, Hit);
	}
	
	FVector CharacterForward = UpdatedComponent->GetForwardVector();
	float DotProduct = FVector::DotProduct(CharacterForward, Normal);
	if (DotProduct > TwoMinConstant::StopThreshold)
	{
		return Super::ComputeSlideVector(Delta, Time, Normal, Hit);
	}

	if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwner()))
	{
		PlayerCharacter->MoveStop(FInputActionValue());
	}
	
	return FVector::ZeroVector;
}
