// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_PerfectGuard_Player.h"

#include "EnhancedPlayerInput.h"
#include "InputActionValue.h"

class UEnhancedPlayerInput;

void UTwoMinGA_PerfectGuard_Player::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_PerfectGuard_Player::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	bool bJumpHeld = false;
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		if (UEnhancedPlayerInput* EPI = Cast<UEnhancedPlayerInput>(PC->PlayerInput))
		{
			const FInputActionValue Val = EPI->GetActionValue(InputActionToCheck);
			bJumpHeld = Val.Get<bool>();
		}
	}

	if (bJumpHeld)
	{
		UTwoMinAbilitySystemComponent* ASC = GetTwoMinAbilitySystemComponentFromActorInfo();
		if (!ASC) return;
		
		if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(GuardAbilityClass))
		{
			ASC->TryActivateAbility(Spec->Handle, true);
		}
	}
}
