// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_Guard_Player.h"

#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "AbilitySystem/Ability/Task/TwoMinAT_UpdateRotation.h"
#include "Camera/CameraComponent.h"
#include "Character/TwoMinPlayerCharacter.h"

void UTwoMinGA_Guard_Player::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter)
	{
		CustomCancelAbility();
		return;
	}
	
	// Lock On 중 회전 하지 않음.
	if (UTwoMinFunctionLibrary::HasGameplayTag(PlayerCharacter, TwoMinGameplayTag::Player_State_LockOn) == false)
	{
		UpdateRotationTickTask = UTwoMinAT_UpdateRotation::CreateTickTask(this);
		if (!UpdateRotationTickTask)
		{
			CustomCancelAbility();
			return;
		}
		
		CurTime = 0;
		UpdateRotationTickTask->OnUpdateRotationTick.AddUniqueDynamic(this, &UTwoMinGA_Guard_Player::OnUpdateRotationTick);
		UpdateRotationTickTask->MaxDuration = RotationTime;
		UpdateRotationTickTask->ReadyForActivation();
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UTwoMinGA_Guard_Player::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const float CurrentStamina = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentStaminaAttribute());
	const bool bIsEnoughStamina = CurrentStamina >= 0;
	return bIsEnoughStamina;
}

void UTwoMinGA_Guard_Player::OnUpdateRotationTick(float DeltaSeconds)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;
	
	CurTime += DeltaSeconds;
	FRotator CurrentRotator = PlayerCharacter->GetActorRotation();
	FVector CameraForward = PlayerCharacter->GetCamera()->GetForwardVector();
	CameraForward.Z = 0.f;
	
	float Ratio = CurTime / RotationTime;
	FRotator NewRotation = FMath::Lerp(CurrentRotator, CameraForward.Rotation(), Ratio);
	
	PlayerCharacter->SetActorRotation(NewRotation);
}
