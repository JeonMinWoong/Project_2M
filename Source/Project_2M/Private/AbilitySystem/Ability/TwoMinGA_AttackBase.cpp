// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"

#include "TwoMinDebugHelper.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

void UTwoMinGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	if (AttackMontages.IsEmpty()) return;

	if (CurComboCount > MaxComboCount)
	{
		CurComboCount = 1;
	}

	if (!AttackMontages.Contains(CurComboCount)) return;


	if (UAnimMontage* MontageToPlay = AttackMontages[CurComboCount])
	{
		PlayToAnimMontage(MontageToPlay);
		//UE_LOG(LogTemp, Warning, TEXT("Ability this: %p"), static_cast<const void*>(this));
		//UE_LOG(LogTemp, Warning, TEXT("Increase ptr: %p"), static_cast<const void*>(&CurComboCount));
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_AttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateEndAbility, bool bWasCancelled)
{
	//ResetComboCount();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UTwoMinGA_AttackBase::UTwoMinGA_AttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	UE_LOG(LogTemp, Warning, TEXT("AttackBase Constructor | this = %p"), this);
}

void UTwoMinGA_AttackBase::AddComboCount()
{
	CurComboCount++;
	UE_LOG(LogTemp, Warning, TEXT("Ability this: %p"), static_cast<const void*>(this));
	UE_LOG(LogTemp, Warning, TEXT("Decrease ptr: %p"), static_cast<const void*>(&CurComboCount));
}

void UTwoMinGA_AttackBase::ResetComboCount()
{
	CurComboCount = 1;
	UE_LOG(LogTemp, Warning, TEXT("Ability this: %p"), static_cast<const void*>(this));
	UE_LOG(LogTemp, Warning, TEXT("Decrease ptr: %p"), static_cast<const void*>(&CurComboCount));
}
