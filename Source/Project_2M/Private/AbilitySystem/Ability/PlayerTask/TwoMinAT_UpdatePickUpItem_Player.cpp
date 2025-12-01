// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/PlayerTask/TwoMinAT_UpdatePickUpItem_Player.h"

UTwoMinAT_UpdatePickUpItem_Player* UTwoMinAT_UpdatePickUpItem_Player::CreateTickTask(UGameplayAbility* OwningAbility)
{
	UTwoMinAT_UpdatePickUpItem_Player* Task = NewAbilityTask<UTwoMinAT_UpdatePickUpItem_Player>(OwningAbility);
	return Task; 
}

void UTwoMinAT_UpdatePickUpItem_Player::Activate()
{
	bTickingTask = true;
}

void UTwoMinAT_UpdatePickUpItem_Player::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (OnPickUpItemTick.IsBound())
	{
		OnPickUpItemTick.Broadcast(DeltaTime);	
	}
}