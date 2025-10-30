// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_HitReactBase.h"

#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "Character/TwoMinBaseCharacter.h"
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

	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	ATwoMinBaseCharacter* InstigatorCharacter = Cast<ATwoMinBaseCharacter>(TriggerEventData->Instigator);
	if (!MyCharacter || !InstigatorCharacter)
	{
		CustomCancelAbility();
		return;
	}
	
	const FAttackInfoData& AttackInfoData = AttackPayload->Data;
	const FVector OwnerForward = MyCharacter->GetActorForwardVector();
	const FVector ToImpact = (InstigatorCharacter->GetActorLocation() - MyCharacter->GetActorLocation()).GetSafeNormal();
	const int32 HitReactNumber = GetPlayHitReactMontageNumber(OwnerForward, ToImpact, AttackInfoData);
	
	PlayToAnimMontage(HitReactMontages[HitReactNumber]);
	OnStartKnockBack(MyCharacter, HitReactMontages[HitReactNumber], ToImpact, AttackInfoData.HitData.PushDistance,
			AttackInfoData.HitData.PushTime, AttackInfoData.HitData.KnockBackCurve);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_HitReactBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ATwoMinBaseCharacter* Character = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	Character->GetMotionWarpingComponent()->RemoveWarpTarget(FName("KB_Target"));
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

int32 UTwoMinGA_HitReactBase::GetPlayHitReactMontageNumber(const FVector OwnerForward, const FVector ToImpact,
	const FAttackInfoData& AttackInfoData) const
{
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