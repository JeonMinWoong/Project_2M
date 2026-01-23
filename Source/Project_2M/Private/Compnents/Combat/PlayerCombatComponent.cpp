// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/Combat/PlayerCombatComponent.h"

#include "Compnents/AutoTargetingComponent.h"

UPlayerCombatComponent::UPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	AutoTargetComponent = CreateDefaultSubobject<UAutoTargetingComponent>(TEXT("AutoTargetComponent"));
	ExecutionComponent = CreateDefaultSubobject<UExecutionComponent>(TEXT("ExecutionComponent"));
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* HitActor, int32 HitWeaponIndex)
{
	Super::OnHitTargetActor(HitActor, HitWeaponIndex);
}

UAutoTargetingComponent* UPlayerCombatComponent::GetAutoTargetingComponent() const
{
	return AutoTargetComponent;
}

UExecutionComponent* UPlayerCombatComponent::GetExecutionComponent() const
{
	return ExecutionComponent;
}