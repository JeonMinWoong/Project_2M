// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Task/TwoMinAT_UpdateAiming.h"

UTwoMinAT_UpdateAiming* UTwoMinAT_UpdateAiming::CreateTickTask(UGameplayAbility* OwningAbility)
{
	UTwoMinAT_UpdateAiming* Task = NewAbilityTask<UTwoMinAT_UpdateAiming>(OwningAbility);
	return Task; 
}

void UTwoMinAT_UpdateAiming::Activate()
{
	bTickingTask = true;
}

void UTwoMinAT_UpdateAiming::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (OnAimingTick.IsBound())
	{
		OnAimingTick.Broadcast(DeltaTime);	
	}
}
