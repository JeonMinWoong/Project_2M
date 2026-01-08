// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Task/TwoMinAT_TeleportTaskBase.h"

UTwoMinAT_TeleportTaskBase* UTwoMinAT_TeleportTaskBase::CreateTickTask(UGameplayAbility* OwningAbility, 
	float InTeleportStartDelay, float InTeleportFinishDelay)
{
	UTwoMinAT_TeleportTaskBase* Task = NewAbilityTask<UTwoMinAT_TeleportTaskBase>(OwningAbility);
	Task->TeleportStartDelay = InTeleportStartDelay;
	Task->TeleportFinishDelay = InTeleportFinishDelay;
	
	return Task; 
}


void UTwoMinAT_TeleportTaskBase::Activate()
{
	bTickingTask = true;
	ElapsedTime = 0.f;
}

void UTwoMinAT_TeleportTaskBase::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	if (FOnStartTeleport.IsBound() == false) return;
	if (FOnFinishTeleport.IsBound() == false) return;
	
	ElapsedTime += DeltaTime;
	if (bIsTeleportStarted == false)
	{
		if (ElapsedTime < TeleportStartDelay) return;
		
		bIsTeleportStarted = true;
		FOnStartTeleport.Broadcast();
	}
	else
	{
		if (ElapsedTime < TeleportFinishDelay) return;
		
		FOnFinishTeleport.Broadcast();
		EndTask();
	}
}
