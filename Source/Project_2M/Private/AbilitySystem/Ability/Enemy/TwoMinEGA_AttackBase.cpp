// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinEGA_AttackBase.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/TwoMinEnemyDummy.h"

void UTwoMinEGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	if (AttackMontage == nullptr)
	{
		CustomCancelAbility();
		return;
	}
	
	if(ATwoMinEnemyDummy* Dummy = Cast<ATwoMinEnemyDummy>(GetAvatarActorFromActorInfo()))
	{
		StartLocation = Dummy->GetActorLocation();
	}

	PlayToAnimMontage(AttackMontage);

	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, OnHitEventTag, nullptr, false, true);

	Task->EventReceived.AddDynamic(this, &ThisClass::OnAttackGameplayEventReceived);
	Task->ReadyForActivation();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinEGA_AttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{

	if(ATwoMinEnemyDummy* Dummy = Cast<ATwoMinEnemyDummy>(GetAvatarActorFromActorInfo()))
	{
		Dummy->SetActorLocation(StartLocation);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

TSubclassOf<UGameplayEffect> UTwoMinEGA_AttackBase::GetAttackGameplayEffectClass() const
{
	return AttackGameplayEffectClass;
}
