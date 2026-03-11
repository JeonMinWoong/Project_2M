// Fill out your copyright notice in the Description page of Project Settings.


#include "System/InteractionActor_MapSelect.h"

#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/PlayerUIComponent.h"

void AInteractionActor_MapSelect::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionType = EInteractionType::MapSelect;
}

void AInteractionActor_MapSelect::Interact(ATwoMinPlayerCharacter* PlayerCharacter)
{
	Super::Interact(PlayerCharacter);
	
	UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetPlayerUIComponent();
	if (PlayerUIComponent->IsMapSelectWidgetOpen())	return;
	
	PlayerUIComponent->OpenMapSelectWidget(PlayerCharacter, true);
}
