// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoMinGameplayTag.h"

namespace TwoMinGameplayTag
{
	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_OneHand, "InputTag.LightAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_OneHand, "InputTag.HeavyAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchTarget, "InputTag.SwitchTarget");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Roll, "InputTag.Roll");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle, "InputTag.Toggle");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle_Run, "InputTag.Toggle.Run");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle_LockOn, "InputTag.Toggle.LockOn");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHold, "InputTag.MustBeHold");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHold_Guard, "InputTag.MustBeHold.Guard");

	// Weapon Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Sword, "Player.Weapon.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Shield, "Player.Weapon.Shield");

	// Player Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LightAttack_OneHand, "Player.Ability.LightAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HeavyAttack_OneHand, "Player.Ability.HeavyAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LockOn, "Player.Ability.LockOn");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Roll, "Player.Ability.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Guard, "Player.Ability.Guard");

	// Player Event Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Left, "Player.Event.SwitchTarget.Left");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Right, "Player.Event.SwitchTarget.Right");
	
	// Player State Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_State_LockOn, "Player.State.LockOn");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Rolling, "Player.State.Rolling");

	// Enemy Weapon Tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");

	// Enemy Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");

	// Shard Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Guard, "Shared.Ability.Guard");
	
	// Shard Event Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_BeforeGuard, "Shared.Event.BeforeGuard");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_KeepGuard, "Shared.Event.KeepGuard");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_AfterGuard, "Shared.Event.AfterGuard");
	
	// Shard State Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Invincible, "Shared.State.Invincible");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Guarding, "Shared.State.Guarding");
}