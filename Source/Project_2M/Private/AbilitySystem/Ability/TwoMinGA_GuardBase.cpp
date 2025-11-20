// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToMinTypes/TwoMinStructTypes.h"

class UAbilityTask_WaitGameplayEvent;

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
	
  	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_GuardBase::CustomEventReceived(FGameplayEventData Payload)
{
	if (Payload.EventTag == BeforeGuardEventTag)
	{
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

float UTwoMinGA_GuardBase::CustomCalculationStaminaCost(const EAttackType AttackType) const
{
	if (!StaminaCostCurveTable || GuardStaminaCostNameMap.IsEmpty()) return 0.f;

	int32 Level = GetAbilityLevel();
	const FString& LevelString = FString::FromInt(Level);
	const FRealCurve* Curve = StaminaCostCurveTable->FindCurve(GuardStaminaCostNameMap[AttackType], LevelString);
	if (!Curve) return 0.f;
	
	const float StaminaCost = Curve->Eval(Level);
	if (StaminaCost <= 0.f) return 0.f;

	return StaminaCost;
}

void UTwoMinGA_GuardBase::CustomApplyCost(const EAttackType AttackType) const
{
	const float StaminaCost = CustomCalculationStaminaCost(AttackType);
	const UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (!CostGE) return;
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(
		CostGE->GetClass(),
		GetAbilityLevel(),
		ASC->MakeEffectContext()
	);

	const float RemainStamina = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentStaminaAttribute()) - StaminaCost;
	
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Cost_Stamina_Enough, -StaminaCost);

	ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());

	if (RemainStamina < 0)
	{
		bIsEndAbilitySendToExhaustedEvent = true;
	}
}

void UTwoMinGA_GuardBase::OnHitGuard(FGameplayEventData Payload)
{
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

	AActor* InstigatorActor = const_cast<AActor*>(Payload.Instigator.Get());
	if (!InstigatorActor) return;
	
	ATwoMinBaseCharacter* InstigatorCharacter = Cast<ATwoMinBaseCharacter>(InstigatorActor);
	if (!MyCharacter || !InstigatorCharacter)
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
	
	const FAttackInfoData& AttackInfoData = AttackPayload->Data;
	FVector HitPos = InstigatorCharacter->GetActorLocation();
	if (const UProjectilePayloadObject* ProjectilePayload = Cast<UProjectilePayloadObject>(Payload.OptionalObject2))
	{
		HitPos = ProjectilePayload->Data.ProjectileHitPos;
	}
	
	const FVector ToImpact = (HitPos - MyCharacter->GetActorLocation()).GetSafeNormal();
	
	bIsHitGuard = true;
	CustomApplyCost(AttackInfoData.AttackType);
	
	const int32 HitMontageNumber = GetHitMontageNumber(AttackInfoData);

	const FGuardHitData& GuardHitData = GuardHitDataMap[AttackInfoData.AttackType];
	float PushDistance = GuardHitData.PushDistance;
	float PushTime = GuardHitData.PushTime;
	UCurveFloat* PushCurve = GuardHitData.KnockBackCurve;
	if (bIsEndAbilitySendToExhaustedEvent)
	{
		PushDistance = AttackInfoData.HitData.PushDistance;
		PushTime = AttackInfoData.HitData.PushTime;
		PushCurve = AttackInfoData.HitData.KnockBackCurve;
	}
	
	OnStartKnockBack(MyCharacter, HitGuardAnimMontage[HitMontageNumber], ToImpact, PushDistance,
		PushTime, PushCurve);
	
	UAbilityTask_PlayMontageAndWait* Hit = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, HitGuardAnimMontage[HitMontageNumber], 1.f,
		NAME_None, true);

	Hit->OnCompleted.AddDynamic(this, &ThisClass::OnHitEnd);
	Hit->OnBlendOut.AddDynamic(this, &ThisClass::OnHitEnd);
	Hit->OnInterrupted.AddDynamic(this, &ThisClass::OnHitEnd);
	Hit->OnCancelled.AddDynamic(this, &ThisClass::OnHitEnd);

	Hit->ReadyForActivation();
}

int UTwoMinGA_GuardBase::GetHitMontageNumber(const FAttackInfoData& AttackInfoData) const
{
	if (bIsEndAbilitySendToExhaustedEvent)
	{
		return HitGuardAnimMontage.Num() - 1;	
	}
	
	return FMath::Clamp(static_cast<int32>(AttackInfoData.AttackType) - 1, 0,HitGuardAnimMontage.Num() - 1);
}

void UTwoMinGA_GuardBase::OnHitEnd()
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter)
	{
		CustomCancelAbility();
		return;
	}

	if (bIsGuard == false || bIsEndAbilitySendToExhaustedEvent)
	{
		CustomCancelAbility();
		return;
	}

	bIsHitGuard = false;
	PlayToAnimMontage(GuardAnimMontage, FName("Guard_Loop"));
}
