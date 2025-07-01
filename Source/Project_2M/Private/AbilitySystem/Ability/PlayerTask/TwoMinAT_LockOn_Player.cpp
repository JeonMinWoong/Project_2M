// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PlayerTask/TwoMinAT_LockOn_Player.h"

UTwoMinAT_LockOn_Player* UTwoMinAT_LockOn_Player::CreateTickTask(UGameplayAbility* OwningAbility)
{
	UTwoMinAT_LockOn_Player* Task = NewAbilityTask<UTwoMinAT_LockOn_Player>(OwningAbility);
	return Task; 
}

void UTwoMinAT_LockOn_Player::Activate()
{
	bTickingTask = true;
}

void UTwoMinAT_LockOn_Player::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (OnLockOnTick.IsBound())
	{
		OnLockOnTick.Broadcast(DeltaTime);	
	}
}
