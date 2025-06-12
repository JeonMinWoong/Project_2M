// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace TwoMinGameplayTag
{
	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_OneHand);
	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_OneHand);

	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggle);		// Toggle
	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggle_Run);

	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHold);	// Hold

	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Sword);
	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Shield);

	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_LightAttack_OneHand);
	PROJECT_2M_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_HeavyAttack_OneHand);
}
