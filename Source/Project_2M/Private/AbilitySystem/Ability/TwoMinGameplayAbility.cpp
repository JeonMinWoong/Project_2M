// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEGA_AttackBase.h"
#include "Camera/CameraComponent.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "ToMinTypes/TwoMinStructTypes.h"

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

void UTwoMinGameplayAbility::PlayToAnimMontage(UAnimMontage* AnimMontage)
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, AnimMontage, 1.f, NAME_None, false
	);

	if (!Task) return;
	
	Task->OnCompleted.AddDynamic(this, &ThisClass::CustomEndAbility);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::CustomEndAbility);
	Task->OnCancelled.AddDynamic(this, &ThisClass::CustomEndAbility);

	Task->ReadyForActivation();
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

	AActor* TargetCharacter = Cast<AActor>(Payload.Target);
	if (!TargetCharacter) return;
	
	Payload.OptionalObject = AttackPayload;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		TargetCharacter,
		TwoMinGameplayTag::Shared_Event_HitReact,
		Payload
	);
}

void UTwoMinGameplayAbility::CustomEndAbility()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
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


