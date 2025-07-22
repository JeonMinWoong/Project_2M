// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/Combat/PlayerCombatComponent.h"

#include "Compnents/AutoTargetingComponent.h"

UPlayerCombatComponent::UPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	AutoTargetComponent = CreateDefaultSubobject<UAutoTargetingComponent>(TEXT("AutoTargetComponent"));
}

UAutoTargetingComponent* UPlayerCombatComponent::GetAutoTargetingComponent() const
{
	return AutoTargetComponent;
}
