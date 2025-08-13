// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_HitReactBase.h"

#include "TwoMinDebugHelper.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinGA_HitReactBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	if (HitReactMontages.IsEmpty())
	{
		CustomCancelAbility();
		return;
	}

	if (!TriggerEventData->OptionalObject->IsValidLowLevel())
	{
		CustomCancelAbility();
		return;
	}

	const UAttackPayloadObject* AttackPayload = Cast<UAttackPayloadObject>(TriggerEventData->OptionalObject);
	if (!AttackPayload)
	{
		CustomCancelAbility();
		return;
	}

	const FAttackInfoData& AttackInfoData = AttackPayload->Data;
	int32 HitReactNumber = GetPlayHitReactMontageNumber(GetAvatarActorFromActorInfo(),
		TriggerEventData->Instigator, AttackInfoData);
	PlayToAnimMontage(HitReactMontages[HitReactNumber]);

	DebugTwoMin::Print(FString::Printf(TEXT("Attacker : %s"), *TriggerEventData->Instigator->GetActorLabel()),
		FColor::Green);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_HitReactBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

int32 UTwoMinGA_HitReactBase::GetPlayHitReactMontageNumber(const AActor* OwnerActor, const AActor* AttackerActor,
	const FAttackInfoData& AttackInfoData) const
{
	const FVector OwnerForward = OwnerActor->GetActorForwardVector();
	const FVector ToImpact = (AttackerActor->GetActorLocation() - OwnerActor->GetActorLocation()).GetSafeNormal();
	const float AngleDeg = UKismetMathLibrary::DegAcos(FVector::DotProduct(OwnerForward, ToImpact));
	
	const float Sign = FMath::Sign(FVector::CrossProduct(OwnerForward, ToImpact).Z);

	float FinalAngle = AngleDeg * Sign; // -180 ~ 180
	if (FinalAngle >= -45 && FinalAngle <= 45)
	{
		return AttackInfoData.HitDirectionNumber; 
	}

	if (FinalAngle > 45 && FinalAngle <= 135)
	{
		return 12;	// 우측
	}

	if (FinalAngle < -45 && FinalAngle >= -135)
	{
		return 11;	// 좌측
	}
	
	return 10;	// 후방
}
