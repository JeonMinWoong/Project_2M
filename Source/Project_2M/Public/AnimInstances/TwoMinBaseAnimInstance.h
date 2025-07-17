// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TwoMinBaseAnimInstance.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag GamePlayTag) const;
};
