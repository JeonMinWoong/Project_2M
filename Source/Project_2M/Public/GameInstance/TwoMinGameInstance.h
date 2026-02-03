// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TwoMinGameInstance.generated.h"

class UWorldStageManager;
class UItemDataManager;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	UItemDataManager* ItemDataManager;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	UWorldStageManager* StateManager;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsStageMoving = false;
	
};
