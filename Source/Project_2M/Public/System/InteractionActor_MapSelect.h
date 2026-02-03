// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/InteractionActorBase.h"
#include "InteractionActor_MapSelect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API AInteractionActor_MapSelect : public AInteractionActorBase
{
	GENERATED_BODY()
	
public:
	virtual void Interact(ATwoMinPlayerCharacter* PlayerCharacter) override;
};
