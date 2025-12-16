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
	UE_DEFINE_GAMEPLAY_TAG(InputTag_RunAttack_OneHand, "InputTag.RunAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchTarget, "InputTag.SwitchTarget");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Roll, "InputTag.Roll");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ItemPickUp, "InputTag.ItemPickUp");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Execution_OneHand, "InputTag.Execution.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_OpenInventory, "InputTag.OpenInventory");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UseItem, "InputTag.UseItem");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LeftQuickSlot, "InputTag.LeftQuickSlot");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_RightQuickSlot, "InputTag.RightQuickSlot");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle, "InputTag.Toggle");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle_LockOn, "InputTag.Toggle.LockOn");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHold, "InputTag.MustBeHold");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHold_Run, "InputTag.MustBeHold.Run");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHold_Guard, "InputTag.MustBeHold.Guard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHold_ChargeAttack_OneHand, "InputTag.MustBeHold.ChargeAttack.OneHand");

	// Weapon Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Sword, "Player.Weapon.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Shield, "Player.Weapon.Shield");

	// Player Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LightAttack_OneHand, "Player.Ability.LightAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HeavyAttack_OneHand, "Player.Ability.HeavyAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_CountAttack_OneHand, "Player.Ability.CounterAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_ChargeAttack_OneHand, "Player.Ability.ChargeAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_RunAttack_OneHand, "Player.Ability.RunAttack.OneHand");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LockOn, "Player.Ability.LockOn");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Roll, "Player.Ability.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_ItemPickUp, "Player.Ability.ItemPickUp");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_UseItem, "Player.Ability.UseItem");

	// Player Event Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Left, "Player.Event.SwitchTarget.Left");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Right, "Player.Event.SwitchTarget.Right");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ChargingUp, "Player.Event.ChargingUp");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ChargingEnd, "Player.Event.ChargingEnd");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ItemPickUp, "Player.Event.ItemPickUp");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_UseItem, "Player.Event.UseItem");
	
	// Player State Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Running, "Player.State.Running");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_LockOn, "Player.State.LockOn");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Rolling, "Player.State.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Charging, "Player.State.Charging");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Attacking, "Player.State.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_ItemPickUp, "Player.State.ItemPickUp");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_UseItem, "Player.State.UseItem");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_State_OpenInventory, "Player.State.OpenInventory");

	// Enemy Weapon Tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");

	// Enemy Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_1, "Enemy.Ability.Melee.1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_2, "Enemy.Ability.Melee.2");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_3, "Enemy.Ability.Melee.3");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_4, "Enemy.Ability.Melee.4");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_5, "Enemy.Ability.Melee.5");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_6, "Enemy.Ability.Melee.6");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_100, "Enemy.Ability.Melee.100");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Range, "Enemy.Ability.Range");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Range_1, "Enemy.Ability.Range.1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Range_2, "Enemy.Ability.Range.2");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Range_3, "Enemy.Ability.Range.3");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Detect, "Enemy.Ability.Detect");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Groggy, "Enemy.Ability.Groggy");

	// Enemy Cooldown Tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Melee_1, "Enemy.Cooldown.Ability.Melee.1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Melee_2, "Enemy.Cooldown.Ability.Melee.2");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Melee_3, "Enemy.Cooldown.Ability.Melee.3");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Melee_4, "Enemy.Cooldown.Ability.Melee.4");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Melee_5, "Enemy.Cooldown.Ability.Melee.5");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Melee_6, "Enemy.Cooldown.Ability.Melee.6");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Melee_100, "Enemy.Cooldown.Ability.Melee.100");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Range_1, "Enemy.Cooldown.Ability.Range.1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Range_2, "Enemy.Cooldown.Ability.Range.2");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_Ability_Range_3, "Enemy.Cooldown.Ability.Range.3");

	// Enemy Event Tag
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Groggy, "Enemy.Event.Groggy");
	
	// Enemy State Tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_State_Running, "Enemy.State.Running");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_State_Groggy, "Enemy.State.Groggy");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_State_DecreaseGroggy, "Enemy.State.DecreaseGroggy");
	
	// Shard Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReactDown, "Shared.Ability.HitReactDown");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReactThrow, "Shared.Ability.HitReactThrow");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Guard, "Shared.Ability.Guard");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Exhausted, "Shared.Ability.Exhausted");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_PerfectGuard, "Shared.Ability.PerfectGuard");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Execution_Caster, "Shared.Ability.Execution.Caster");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Execution_Target, "Shared.Ability.Execution.Target");

	// Shard SetByCaller
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_ExecutionDamage, "Shared.SetByCaller.ExecutionDamage");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_GaurdSuccess, "Shared.SetByCaller.GuardSuccess");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_GroggyAmount, "Shared.SetByCaller.GroggyAmount");
	
	// Shard Event Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReactDown, "Shared.Event.HitReactDown");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReactThrow, "Shared.Event.HitReactThrow");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitGuard, "Shared.Event.HitGuard");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Exhausted, "Shared.Event.Exhausted");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SuccessPerfectGuard, "Shared.Event.SuccessPerfectGuard");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Death, "Shared.Event.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SpawnProjectile, "Shared.Event.SpawnProjectile");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_ResetAttackCount, "Shared.Event.ResetAttackCount");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Execution_Caster, "Shared.Event.Execution.Caster");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Execution_Target, "Shared.Event.Execution.Target");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Execution_Death, "Shared.Event.Execution.Death");
	
	// Shard State Tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Invincible, "Shared.State.Invincible");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_UnBreakAttack, "Shared.State.UnBreakAttack");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Guarding, "Shared.State.Guarding");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Exhausted, "Shared.State.Exhausted");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_StaminaNoRegen, "Shared.State.StaminaNoRegen");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_FullStamina, "Shared.State.FullStamina");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Throwing, "Shared.State.Throwing");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Hitting, "Shared.State.Hitting");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_HitDowning, "Shared.State.HitDowning");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_HitThrowing, "Shared.State.HitThrowing");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Death, "Shared.State.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_PerfectGuarding, "Shared.State.PerfectGuarding");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_SuccessPerfectGuarding, "Shared.State.SuccessPerfectGuarding");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_SuperArmor, "Shared.State.SuperArmor");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Execution_Caster, "Shared.State.Execution.Caster");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_Execution_Target, "Shared.State.Execution.Target");
	UE_DEFINE_GAMEPLAY_TAG(Shared_State_PossibleExecution, "Shared.State.PossibleExecution");
	
	// Data Tags
	UE_DEFINE_GAMEPLAY_TAG(Data_Cost_Stamina_Enough, "Data.Cost.Stamina.Enough");
	UE_DEFINE_GAMEPLAY_TAG(Data_Cost_Stamina_UnEnough, "Data.Cost.Stamina.UnEnough");
	UE_DEFINE_GAMEPLAY_TAG(Data_Gain_Experience, "Data.Gain.Experience");
	UE_DEFINE_GAMEPLAY_TAG(Data_Gain_Gold, "Data.Gain.Gold");
	UE_DEFINE_GAMEPLAY_TAG(Data_Gain_Health, "Data.Gain.Health");

	// GameplayEffect Tags
	UE_DEFINE_GAMEPLAY_TAG(GE_Player_Stats, "GE.Player.Stats");
	
}