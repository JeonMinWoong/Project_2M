// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_LightAttack.h"

void UTwoMinGA_LightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (LightAttackMontages.IsEmpty()) return;

	if (CurComboCount > MaxComboCount)
	{
		CurComboCount = 1;
	}

	if (!LightAttackMontages.Contains(CurComboCount)) return;


	if (UAnimMontage* MontageToPlay = LightAttackMontages[CurComboCount])
	{
		CancelComboTimer();
		PlayToAnimMontage(MontageToPlay);
		CurComboCount++;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_LightAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	StartComboTimer();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_LightAttack::StartComboTimer()
{
	// Todo: Actor 에서 해야 할 듯?
	GetWorld()->GetTimerManager().SetTimer(
		StartComboTimerHandle, this, &ThisClass::ResetComboCount, ComboResetTime, false);
}

void UTwoMinGA_LightAttack::CancelComboTimer()
{
	if (!StartComboTimerHandle.IsValid()) return;
	
	GetWorld()->GetTimerManager().ClearTimer(StartComboTimerHandle);
}

void UTwoMinGA_LightAttack::ResetComboCount()
{
	CurComboCount = 1;
}
