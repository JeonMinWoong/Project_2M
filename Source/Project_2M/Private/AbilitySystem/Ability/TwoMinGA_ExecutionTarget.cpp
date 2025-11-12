// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_ExecutionTarget.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"

void UTwoMinGA_ExecutionTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	const UExecutionPayloadObject* ExecutionPayloadObject = Cast<UExecutionPayloadObject>(TriggerEventData->OptionalObject);
	if (!ExecutionPayloadObject)
	{
		CustomCancelAbility();
		return;
	}

	UAnimMontage* ExecutionMontage =
		GetExecutionMontage(ExecutionPayloadObject->bIsExecutionForward, ExecutionPayloadObject->ExecutionNumber);
	if (!ExecutionMontage)
	{
		CustomCancelAbility();
		return;
	}

	AActor* CasterActor = const_cast<AActor*>(TriggerEventData->Instigator.Get());
	if (!CasterActor)
	{
		CustomCancelAbility();
		return;
	}

	ATwoMinBaseCharacter* CasterCharacter = Cast<ATwoMinBaseCharacter>(CasterActor);
	if (!CasterCharacter)
	{
		CustomCancelAbility();
		return;
	}

	ExecutionCaster = CasterCharacter;
	
	PlayToAnimMontage(ExecutionMontage);
	
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, OnExecutionDeathTag, nullptr, false, true);

	Task->EventReceived.AddDynamic(this, &ThisClass::OnExecutionDeathGameplayEventReceive);
	Task->ReadyForActivation();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_ExecutionTarget::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UAnimMontage* UTwoMinGA_ExecutionTarget::GetExecutionMontage(bool bIsExecutionForward, int32 ExecutionNumber)
{
	if (bIsExecutionForward)
	{
		return ExecutionForwardTargetMontages[ExecutionNumber];
	}

	return ExecutionBackTargetMontages[ExecutionNumber];
}

void UTwoMinGA_ExecutionTarget::OnExecutionDeathGameplayEventReceive(FGameplayEventData Payload)
{
	AActor* MyActor = const_cast<AActor*>(Payload.Instigator.Get());
	if (!MyActor) return;

	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(MyActor);
	if (!MyCharacter) return;

	if (!ExecutionCaster) return;

	UTwoMinAbilitySystemComponent* TargetASC = MyCharacter->GetAbilitySystemComponent();
	float CurrentHealth = TargetASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentHealthAttribute());
	if (CurrentHealth > 0.0f) return;
	
	MyCharacter->DeathProcess();
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(ExecutionCaster);
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(MyCharacter);
	if (!PlayerCharacter || !EnemyCharacter) return;

	PlayerCharacter->GetAbilitySystemComponent()->GiveExperience(EnemyCharacter);
}
