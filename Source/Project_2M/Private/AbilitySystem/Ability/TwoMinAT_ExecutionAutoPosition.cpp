// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinAT_ExecutionAutoPosition.h"

UTwoMinAT_ExecutionAutoPosition* UTwoMinAT_ExecutionAutoPosition::CreateTickTask(UGameplayAbility* OwningAbility)
{
	UTwoMinAT_ExecutionAutoPosition* Task = NewAbilityTask<UTwoMinAT_ExecutionAutoPosition>(OwningAbility);
	return Task; 
}

void UTwoMinAT_ExecutionAutoPosition::Activate()
{
	bTickingTask = true;
}

void UTwoMinAT_ExecutionAutoPosition::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	CurrentDuration += DeltaTime;
	if (CurrentDuration >= MaxDuration)
	{
		CurrentDuration = 0;
		EndTask();	
	}
	
	if (OnExecutionAutoPositionTick.IsBound())
	{
		OnExecutionAutoPositionTick.Broadcast(DeltaTime);	
	}
}
