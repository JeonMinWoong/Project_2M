// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Task/TwoMinAT_UpdateRotation.h"

UTwoMinAT_UpdateRotation* UTwoMinAT_UpdateRotation::CreateTickTask(UGameplayAbility* OwningAbility)
{
	UTwoMinAT_UpdateRotation* Task = NewAbilityTask<UTwoMinAT_UpdateRotation>(OwningAbility);
	return Task; 
}

void UTwoMinAT_UpdateRotation::Activate()
{
	bTickingTask = true;
}

void UTwoMinAT_UpdateRotation::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	CurrentDuration += DeltaTime;
	if (CurrentDuration >= MaxDuration)
	{
		CurrentDuration = 0;
		EndTask();	
	}

	if (OnUpdateRotationTick.IsBound())
	{
		OnUpdateRotationTick.Broadcast(DeltaTime);	
	}
}