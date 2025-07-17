// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/TwoMinBaseAnimInstance.h"
#include "TwoMinPlayerLinkedAnimLayer.generated.h"

class UTwoMinPlayerAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinPlayerLinkedAnimLayer : public UTwoMinBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (NotBlueprintThreadSafe))
	UTwoMinPlayerAnimInstance* GetPlayerAnimInstance() const;
};
