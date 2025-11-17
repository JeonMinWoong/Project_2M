// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_ChargeAttackBase.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Character/TwoMinBaseCharacter.h"

void UTwoMinGA_ChargeAttackBase::PreActivate(const FGameplayAbilitySpecHandle Handle,
											const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
											FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	FGameplayEventData EventData;
	UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
		GetAvatarActorFromActorInfo(),
		TwoMinGameplayTag::Shared_Event_ResetAttackCount,
		EventData
	);

	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UTwoMinGA_ChargeAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	
	UAbilityTask_WaitGameplayEvent* ChargeUpTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, ChargeUpEventTag, nullptr, false, true);
	
	ChargeUpTask->EventReceived.AddDynamic(this, &ThisClass::OnChargeUp);
	ChargeUpTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* ChargeEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, ChargeEndEventTag, nullptr, false, true);
	
	ChargeEndTask->EventReceived.AddDynamic(this, &ThisClass::OnChargeEnd);
	ChargeEndTask->ReadyForActivation();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_ChargeAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	CurChargeCount = 1;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_ChargeAttackBase::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
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
	
	UAnimMontage* Montage = Anim->GetCurrentActiveMontage();
	if (!Montage)
	{
		CustomCancelAbility();
		return;
	}

	if (Anim->Montage_GetCurrentSection(Montage) == FName("Charge_Attack"))
	{
		return;
	}
	
	MyCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Charge_Attack"), Montage);
}

bool UTwoMinGA_ChargeAttackBase::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const float CurrentStamina = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentStaminaAttribute());
	if (!StaminaCostCurveTable || CustomCostName.IsNone())
	{
		return true;	
	}
	
	const bool bIsEnoughStamina = CurrentStamina > 0;
	return bIsEnoughStamina;
}

float UTwoMinGA_ChargeAttackBase::CustomAddStaminaCost() const
{
	return IsFullCharge() ? FullChargeAddStaminaCostValue : 0;
}

float UTwoMinGA_ChargeAttackBase::CustomCalculationStaminaCost() const
{
	if (!StaminaCostCurveTable || CustomCostName.IsNone()) return 0.f;

	int32 Level = GetAbilityLevel();
	const FString& LevelString = FString::FromInt(Level);
	const FRealCurve* Curve = StaminaCostCurveTable->FindCurve(CustomCostName, LevelString);
	if (!Curve) return 0.f;
	
	const float StaminaCost = Curve->Eval(Level);
	if (StaminaCost <= 0.f) return 0.f;

	return StaminaCost + CustomAddStaminaCost();
}

void UTwoMinGA_ChargeAttackBase::CustomApplyCost() const
{
	const float StaminaCost = CustomCalculationStaminaCost();
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


void UTwoMinGA_ChargeAttackBase::OnChargeUp(FGameplayEventData Payload)
{
	if (IsFullCharge()) return;
	
	CurChargeCount++;
}

void UTwoMinGA_ChargeAttackBase::OnChargeEnd(FGameplayEventData Payload)
{
	CustomApplyCost();
}

bool UTwoMinGA_ChargeAttackBase::IsFullCharge() const
{
	return CurChargeCount >= MaxChargeCount;
}
