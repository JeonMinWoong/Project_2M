// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/Combat/PlayerCombatComponent.h"

#include "Compnents/AutoTargetingComponent.h"

UPlayerCombatComponent::UPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	AutoTargetComponent = CreateDefaultSubobject<UAutoTargetingComponent>(TEXT("AutoTargetComponent"));
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Super::OnHitTargetActor(HitActor);
}

UAutoTargetingComponent* UPlayerCombatComponent::GetAutoTargetingComponent() const
{
	return AutoTargetComponent;
}
