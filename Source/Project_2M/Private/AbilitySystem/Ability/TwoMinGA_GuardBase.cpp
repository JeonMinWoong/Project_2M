// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"

#include "TwoMinDebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/TwoMinBaseCharacter.h"

void UTwoMinGA_GuardBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	TwoMinDebugHelper::Print(TEXT("Guard Ability Activated"), FColor::Green);
	
	PlayToAnimMontage(GuardAnimMontage, FName("Guard_Start"));
	WaitGameplayEvent(BeforeGuardEventTag);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_GuardBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	TwoMinDebugHelper::Print(TEXT("Guard Ability Ended"), FColor::Red);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_GuardBase::CustomEventReceived(FGameplayEventData Payload)
{
	if (Payload.EventTag == BeforeGuardEventTag)
	{
		TwoMinDebugHelper::Print(TEXT("Event Received: Before Guard"), FColor::Blue);
		ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(this->GetAvatarActorFromActorInfo());
		if (!MyCharacter)
		{
			CustomCancelAbility();
			return;
		}
		
		MyCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Guard_Loop"), GuardAnimMontage);
	}
}

void UTwoMinGA_GuardBase::CustomCompleteAbility()
{
	
}
