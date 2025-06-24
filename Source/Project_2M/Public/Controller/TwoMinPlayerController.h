// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "TwoMinPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinPlayerController : public APlayerController,  public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ATwoMinPlayerController();
	//~ Begin IGenericTeamAgentInterface Interface.
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamAgentInterface Interface.


private:
	FGenericTeamId PlayerTeamId;
};
