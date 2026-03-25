// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_PerfectGuard_Player.h"

#include "EnhancedPlayerInput.h"
#include "InputActionValue.h"

class UEnhancedPlayerInput;

void UTwoMinGA_PerfectGuard_Player::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter)
	{
		CustomCancelAbility();
		return;
	}
	
	StartOutline(PlayerCharacter);
	CameraShakeToShakeType(GetAvatarActorFromActorInfo(), ECameraShakeType::Heavy);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_PerfectGuard_Player::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	FinishOutline();
	
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

void UTwoMinGA_PerfectGuard_Player::StartOutline(const ATwoMinPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter->GetMesh() || !OutlineMaterial) return;
	
	if (!OutlineDynamicMaterial)
	{
		OutlineDynamicMaterial = UMaterialInstanceDynamic::Create(OutlineMaterial, this);	
	}
		
	OutlineDynamicMaterial->SetScalarParameterValue(TEXT("Outline"), ThicknessValue);
	PlayerCharacter->GetMesh()->SetOverlayMaterial(OutlineDynamicMaterial);
	GetWorld()->GetTimerManager().ClearTimer(OutlineFadeOutTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(OutlineFadeOutTimerHandle, this, 
		&UTwoMinGA_PerfectGuard_Player::FadeOutOutline, 0.01f, true);
}


void UTwoMinGA_PerfectGuard_Player::FadeOutOutline()
{
	if (!OutlineDynamicMaterial) return;
		
	float CurrentOutlineOpacity = 0;
	OutlineDynamicMaterial->GetScalarParameterValue(TEXT("Outline"), CurrentOutlineOpacity);
		
	CurrentOutlineOpacity -= 0.05f;
	OutlineDynamicMaterial->SetScalarParameterValue(TEXT("Outline"), CurrentOutlineOpacity);

	if (CurrentOutlineOpacity > 0.0f) return;
	
	FinishOutline();
}

void UTwoMinGA_PerfectGuard_Player::FinishOutline()
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
		if (!PlayerCharacter || !PlayerCharacter->GetMesh()) return;
			
	PlayerCharacter->GetMesh()->SetOverlayMaterial(nullptr);
	GetWorld()->GetTimerManager().ClearTimer(OutlineFadeOutTimerHandle);
}
