// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEGA_AttackBase.h"
#include "Camera/CameraComponent.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToMinTypes/TwoMinStructTypes.h"

class UAbilityTask_WaitGameplayEvent;

UTwoMinGameplayAbility::UTwoMinGameplayAbility()
{
	bRetriggerInstancedAbility = UTwoMinGameplayAbility::bIsReTriggerSameAbility();
}

void UTwoMinGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EToMinAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UTwoMinGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	bIsReTriggerAble = false;
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EToMinAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}

	PossibleCancelAbilities.Empty();
}

bool UTwoMinGameplayAbility::bIsReTriggerSameAbility() const
{
	return false;
}

void UTwoMinGameplayAbility::PlayToAnimMontage(UAnimMontage* AnimMontage, FName StartSectionName)
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, AnimMontage, 1.f, StartSectionName, false
	);

	if (!Task) return;
	
	Task->OnCompleted.AddDynamic(this, &ThisClass::CustomCompleteAbility);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::CustomInterruptedAbility);
	Task->OnCancelled.AddDynamic(this, &ThisClass::CustomCompleteAbility);

	Task->ReadyForActivation();
}

void UTwoMinGameplayAbility::WaitGameplayEvent(FGameplayTag EventTag, bool bIsOnce)
{
	UAbilityTask_WaitGameplayEvent* EventTask =UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, EventTag, nullptr, bIsOnce, true);

	EventTask->EventReceived.AddDynamic(this, &ThisClass::CustomEventReceived);

	EventTask->ReadyForActivation();
}

void UTwoMinGameplayAbility::CustomEventReceived(FGameplayEventData Payload)
{
	
}

void UTwoMinGameplayAbility::OnAttackGameplayEventReceived(FGameplayEventData Payload)
{
	ATwoMinBaseCharacter* BaseCharacter = Cast<ATwoMinBaseCharacter>(Payload.Instigator);
	if (!BaseCharacter) return;

	const ECharacterType CharacterType = BaseCharacter->GetCharacterType();
	if (CharacterType == ECharacterType::None) return;

	UAttackPayloadObject* AttackPayload = NewObject<UAttackPayloadObject>(BaseCharacter);
	
	if (CharacterType == ECharacterType::Enemy)
	{
		//DebugTwoMin::Print(TEXT("Enemy Ability Event Received"), FColor::Red);

		UTwoMinEGA_AttackBase* EnemyAttackBase = Cast<UTwoMinEGA_AttackBase>(this);
		if (!EnemyAttackBase) return;

		const FAttackInfoData& AttackInfoData = EnemyAttackBase->GetAttackInfoData();
		AttackPayload->Data = AttackInfoData;
	}
	else if (CharacterType == ECharacterType::Player)
	{
		//DebugTwoMin::Print(TEXT("Player Ability Event Received"), FColor::Green);

		UTwoMinGA_AttackBase* PlayerAttackBase = Cast<UTwoMinGA_AttackBase>(this);
		if (!PlayerAttackBase) return;

		const FAttackInfoData& AttackInfoData = PlayerAttackBase->GetAttackInfoData();
		AttackPayload->Data = AttackInfoData;
	}

	ATwoMinBaseCharacter* TargetCharacter = Cast<ATwoMinBaseCharacter>(Payload.Target);
	if (!TargetCharacter) return;
	
	Payload.OptionalObject = AttackPayload;

	bool bIsTargetGuard =
		UTwoMinFunctionLibrary::HasGameplayTag(TargetCharacter, TwoMinGameplayTag::Shared_State_Guarding);
	if (bIsTargetGuard)
	{
  		UTwoMinGameplayAbility* Ability =
			TargetCharacter->GetAbilitySystemComponent()->GetPlayingAbilityTag(TwoMinGameplayTag::Shared_Ability_Guard);
		if (Ability)
		{
			if (UTwoMinGA_GuardBase* GuardAbility = Cast<UTwoMinGA_GuardBase>(Ability))
			{
				bIsTargetGuard = GuardAbility->IsGuardCondition(BaseCharacter, TargetCharacter);
			}
		}
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		TargetCharacter,
		bIsTargetGuard ? TwoMinGameplayTag::Shared_Event_HitGuard : TwoMinGameplayTag::Shared_Event_HitReact,
		Payload
	);
}

void UTwoMinGameplayAbility::CustomCompleteAbility()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGameplayAbility::CustomInterruptedAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
		true);
}

void UTwoMinGameplayAbility::CustomCancelAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
		true);
}

bool UTwoMinGameplayAbility::IsMustBeLikedToGameplayAbility() const
{
	return false;
}

bool UTwoMinGameplayAbility::IsPossibleMustBeHoldAbilityImmediatelyCancel() const
{
	return false;
}

void UTwoMinGameplayAbility::SetReTriggerActive(bool InIsReTriggerActive)
{
	bIsReTriggerAble = InIsReTriggerActive;
}

void UTwoMinGameplayAbility::AddPossibleCancelAbility(UTwoMinGameplayAbility* InAbility)
{
	PossibleCancelAbilities.AddUnique(InAbility);
}

void UTwoMinGameplayAbility::RemovePossibleCancelAbility(UTwoMinGameplayAbility* InAbility)
{
	if (PossibleCancelAbilities.IsEmpty())
	{
		return;
	}

	PossibleCancelAbilities.Remove(InAbility);
}

bool UTwoMinGameplayAbility::IsPossibleCancelAbility(UTwoMinGameplayAbility* InAbility) const
{
	return PossibleCancelAbilities.Contains(InAbility);
}


