// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/TwoMinBaseAnimInstance.h"

#include "GameplayTagContainer.h"
#include "TwoMinFunctionLibrary.h"

bool UTwoMinBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag GamePlayTag) const
{
	if (APawn* Character = TryGetPawnOwner())
	{
		return UTwoMinFunctionLibrary::HasGameplayTag(Character, GamePlayTag);
	}

	return false;
}
