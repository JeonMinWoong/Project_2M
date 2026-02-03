// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/TwoMinGameInstance.h"

#include "Managers/WorldStageManager.h"

void UTwoMinGameInstance::Init()
{
	Super::Init();
	
	StateManager->InitStage();
}
