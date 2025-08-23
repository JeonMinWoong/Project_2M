// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToMinTypes/TwoMinStructTypes.h"

class UAbilityTask_WaitGameplayEvent;

bool UTwoMinGA_GuardBase::IsPossibleMustBeHoldAbilityImmediatelyCancel() const
{
	return false;
}

bool UTwoMinGA_GuardBase::IsGuardCondition(const ATwoMinBaseCharacter* Attacker, const ATwoMinBaseCharacter* Target) const
{
	const FVector ToTarget = (Attacker->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal2D();
	const FVector TargetForward = Target->GetActorForwardVector().GetSafeNormal2D();
	const float Angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(ToTarget, TargetForward));
	if (Angle > GuardAngle)
	{
		return false;
	}

	return true;
}

void UTwoMinGA_GuardBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	TwoMinDebugHelper::Print(TEXT("Guard Ability Activated"), FColor::Green);
	bIsGuard = true;
	
	PlayToAnimMontage(GuardAnimMontage, FName("Guard_Start"));
	WaitGameplayEvent(BeforeGuardEventTag, true);

	UAbilityTask_WaitGameplayEvent* HitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, HitGuardEventTag, nullptr, false, true);
	
	HitEventTask->EventReceived.AddDynamic(this, &ThisClass::OnHitGuard);
	HitEventTask->ReadyForActivation();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_GuardBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsActive() == false) return;
	
	TwoMinDebugHelper::Print(TEXT("Guard Ability Ended"), FColor::Red);
	
  	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_GuardBase::CustomEventReceived(FGameplayEventData Payload)
{
	if (Payload.EventTag == BeforeGuardEventTag)
	{
  		TwoMinDebugHelper::Print(TEXT("Event Received: On Guard"), FColor::Blue);
		ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
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
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter) return;
	
	UAnimInstance* Anim = MyCharacter->GetMesh()->GetAnimInstance();
	if (!Anim) return;
	
	FName CurrentSectionName = Anim->Montage_GetCurrentSection(GuardAnimMontage);
	bool bPlayingMontage = Anim->Montage_IsPlaying(GuardAnimMontage);

	if (CurrentSectionName == FName("Guard_End") || !bPlayingMontage)
	{
		Super::CustomCompleteAbility();	
	}
}

void UTwoMinGA_GuardBase::CustomInterruptedAbility()
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter) return;

	if (bIsHitGuard)
	{
		return;
	}

	Super::CustomInterruptedAbility();
}

void UTwoMinGA_GuardBase::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter)
	{
		CustomCancelAbility();
		return;
	}

	UAnimInstance* Anim = MyCharacter->GetMesh()->GetAnimInstance();
	if (!Anim) return;
	if (Anim->Montage_GetCurrentSection(GuardAnimMontage) == FName("Guard_Start"))
	{
		CustomCancelAbility();
		return;
	}

	bIsGuard = false;
	MyCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Guard_End"), GuardAnimMontage);
}

void UTwoMinGA_GuardBase::OnHitGuard(FGameplayEventData Payload)
{
	TwoMinDebugHelper::Print(TEXT("Hit Guard"), FColor::Orange);
	if (!Payload.OptionalObject->IsValidLowLevel())
	{
		CustomCancelAbility();
		return;
	}
	
	const UAttackPayloadObject* AttackPayload = Cast<UAttackPayloadObject>(Payload.OptionalObject);
	if (!AttackPayload)
	{
		CustomCancelAbility();
		return;
	}

	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter)
	{
		CustomCancelAbility();
		return;
	}
	
	UAnimInstance* Anim = MyCharacter->GetMesh()->GetAnimInstance();
	if (!Anim)
	{
		CustomCancelAbility();
		return;
	}
	
	for (int i = 0; i < HitGuardAnimMontage.Num() - 1; ++i)
	{
		if (Anim->Montage_IsPlaying(HitGuardAnimMontage[i]))
		{
			CustomCancelAbility();
			return;
		}
	}
	
	bIsHitGuard = true;
	const FAttackInfoData& AttackInfoData = AttackPayload->Data;
	const int32 HitMontageNumber = FMath::Clamp(static_cast<int32>(AttackInfoData.AttackType), 0,
		HitGuardAnimMontage.Num() - 1);

	bIsBreakGuard = AttackInfoData.AttackType == EAttackType::Ungaurdable;
	
	UAbilityTask_PlayMontageAndWait* Hit = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, HitGuardAnimMontage[HitMontageNumber], 1.f,
		NAME_None, true);

	Hit->OnCompleted.AddDynamic(this, &ThisClass::OnHitEnd);
	Hit->OnBlendOut.AddDynamic(this, &ThisClass::OnHitEnd);
	Hit->OnInterrupted.AddDynamic(this, &ThisClass::OnHitEnd);
	Hit->OnCancelled.AddDynamic(this, &ThisClass::OnHitEnd);

	Hit->ReadyForActivation();
}

void UTwoMinGA_GuardBase::OnHitEnd()
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter)
	{
		CustomCancelAbility();
		return;
	}

	if (bIsGuard == false || bIsBreakGuard)
	{
		CustomCancelAbility();
		return;
	}

	bIsHitGuard = false;
	PlayToAnimMontage(GuardAnimMontage, FName("Guard_Loop"));
}
