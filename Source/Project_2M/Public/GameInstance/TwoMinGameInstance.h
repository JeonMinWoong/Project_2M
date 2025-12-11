// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TwoMinGameInstance.generated.h"

class UItemDataManager;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	UItemDataManager* ItemDataManager;
};
