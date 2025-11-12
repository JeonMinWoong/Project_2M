// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/Combat/PlayerCombatComponent.h"

#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Compnents/AutoTargetingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerCombatComponent::UPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	AutoTargetComponent = CreateDefaultSubobject<UAutoTargetingComponent>(TEXT("AutoTargetComponent"));
	ExecutionComponent = CreateDefaultSubobject<UExecutionComponent>(TEXT("ExecutionComponent"));
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Super::OnHitTargetActor(HitActor);
}

UAutoTargetingComponent* UPlayerCombatComponent::GetAutoTargetingComponent() const
{
	return AutoTargetComponent;
}

UExecutionComponent* UPlayerCombatComponent::GetExecutionComponent() const
{
	return ExecutionComponent;
}
