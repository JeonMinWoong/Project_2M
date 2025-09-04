// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseUIInterface.generated.h"

class UEnemyUIComponent;
class UPlayerUIComponent;
class UBaseUIComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBaseUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_2M_API IBaseUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UBaseUIComponent* GetBaseUIComponent() const = 0;

	virtual UPlayerUIComponent* GetPlayerUIComponent() const;

	virtual UEnemyUIComponent* GetEnemyUIComponent() const;
	
};
