// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoMinGameplayTag.h"

namespace TwoMinGameplayTag
{
	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_OneHand, "InputTag.LightAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_OneHand, "InputTag.HeavyAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_CounterAttack_OneHand, "InputTag.CounterAttack.OneHand");
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
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_CountAttack_OneHand, "Player.Ability.CounterAttack.OneHand");
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
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_1, "Enemy.Ability.Melee.1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_2, "Enemy.Ability.Melee.2");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_3, "Enemy.Ability.Melee.3");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Range, "Enemy.Ability.Range");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Range_1, "Enemy.Ability.Range.1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Range_2, "Enemy.Ability.Range.2");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Range_3, "Enemy.Ability.Range.3");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Detect, "Enemy.Ability.Detect");

	// Enemy Cooldown Tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Melee_1, "Enemy.Cooldown.Ability.Melee.1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Melee_2, "Enemy.Cooldown.Ability.Melee.2");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Melee_3, "Enemy.Cooldown.Ability.Melee.3");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Range_1, "Enemy.Cooldown.Ability.Range.1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Range_2, "Enemy.Cooldown.Ability.Range.2");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Range_3, "Enemy.Cooldown.Ability.Range.3");
	
	// Shard Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Guard, "Shared.Ability.Guard");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Exhausted, "Shared.Ability.Exhausted");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_PerfectGuard, "Shared.Ability.PerfectGuard");

	// Shard SetByCaller
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_GaurdSuccess, "Shared.SetByCaller.GuardSuccess");
	
	// Shard Event Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitGuard, "Shared.Event.HitGuard");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Exhausted, "Shared.Event.Exhausted");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SuccessPerfectGuard, "Shared.Event.SuccessPerfectGuard");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Death, "Shared.Event.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SpawnProjectile, "Shared.Event.SpawnProjectile");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_ResetAttackCount, "Shared.Event.ResetAttackCount");
	
	// Shard State Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Invincible, "Shared.State.Invincible");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_UnBreakAttack, "Shared.State.UnBreakAttack");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Guarding, "Shared.State.Guarding");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Exhausted, "Shared.State.Exhausted");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_StaminaNoRegen, "Shared.State.StaminaNoRegen");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_FullStamina, "Shared.State.FullStamina");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Throwing, "Shared.State.Throwing");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Hitting, "Shared.State.Hitting");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Death, "Shared.State.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_PerfectGuarding, "Shared.State.PerfectGuarding");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_SuccessPerfectGuarding, "Shared.State.SuccessPerfectGuarding");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_SuperArmor, "Shared.State.SuperArmor");

	// Data Tags
	UE_DEFINE_GAMEPLAY_TAG(Data_Cost_Stamina_Enough, "Data.Cost.Stamina.Enough");
	UE_DEFINE_GAMEPLAY_TAG(Data_Cost_Stamina_UnEnough, "Data.Cost.Stamina.UnEnough");
	UE_DEFINE_GAMEPLAY_TAG(Data_Gain_Experience, "Data.Gain.Experience");

	// GameplayEffect Tags
	UE_DEFINE_GAMEPLAY_TAG(GE_Player_Stats, "GE.Player.Stats");
	
}