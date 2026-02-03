// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PlayerTask/TwoMinAT_UpdateInteraction.h"

UTwoMinAT_UpdateInteraction* UTwoMinAT_UpdateInteraction::CreateTickTask(UGameplayAbility* OwningAbility)
{
	UTwoMinAT_UpdateInteraction* Task = NewAbilityTask<UTwoMinAT_UpdateInteraction>(OwningAbility);
	return Task; 
}

void UTwoMinAT_UpdateInteraction::Activate()
{
	bTickingTask = true;
}

void UTwoMinAT_UpdateInteraction::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (OnInteractionTick.IsBound())
	{
		OnInteractionTick.Broadcast(DeltaTime);	
	}
}