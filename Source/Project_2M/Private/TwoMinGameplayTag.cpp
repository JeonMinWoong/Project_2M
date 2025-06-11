// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoMinGameplayTag.h"

namespace TwoMinGameplayTag
{
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_OneHand, "InputTag.LightAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_OneHand, "InputTag.HeavyAttack.OneHand");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle_Run, "InputTag.Toggle.Run");

	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Sword, "Player.Weapon.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Shield, "Player.Weapon.Shield");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LightAttack_OneHand, "Player.Ability.LightAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HeavyAttack_OneHand, "Player.Ability.HeavyAttack.OneHand");
}