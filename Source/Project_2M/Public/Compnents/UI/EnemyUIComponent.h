// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "EnemyUIComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UEnemyUIComponent : public UBaseUIComponent
{
	GENERATED_BODY()

public:
	FOnStringChangeDelegate OnEnemyStateChanged;
};
