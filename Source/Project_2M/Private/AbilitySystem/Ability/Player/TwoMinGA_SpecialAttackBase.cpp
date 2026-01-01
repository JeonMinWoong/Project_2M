// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_SpecialAttackBase.h"

#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Compnents/AutoTargetingComponent.h"
#include "Compnents/Combat/BaseCombatComponent.h"

void UTwoMinGA_SpecialAttackBase::AddComboCount()
{
	CurComboCount = FMath::Clamp(CurComboCount + 1, 1, MaxComboCount);
	if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		PlayerCharacter->GetCombatComponent()->ClearOverlappingActors();
	}
}

void UTwoMinGA_SpecialAttackBase::ResetComboCount()
{
	CurComboCount = 1;
}

void UTwoMinGA_SpecialAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (AttackMontage == nullptr)
	{
		CustomCancelAbility();
		return;
	}
	
	ATwoMinPlayerCharacter* Player = Cast<ATwoMinPlayerCharacter>(ActorInfo->OwnerActor);
	if (Player)
	{
		Player->CancelInputToggle();
		if (UAutoTargetingComponent* AutoTargeting = Player->GetCombatComponent()->GetAutoTargetingComponent())
		{
			AutoTargeting->StartAutoTargeting();
		}
	}

	PlayToAnimMontage(AttackMontage);
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, OnHitEventTag, nullptr, false, true);

	Task->EventReceived.AddDynamic(this, &ThisClass::OnAttackGameplayEventReceivedByMelee);
	Task->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* ResetTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, TwoMinGameplayTag::Shared_Event_ResetAttackCount, nullptr, false,
	true);

	ResetTask->EventReceived.AddDynamic(this, &ThisClass::OnResetAttackCountGameplayEffectReceive);
	ResetTask->ReadyForActivation();
	
	if (HitCollisionMap.IsEmpty() == false)
	{
		WaitGameplayEvent(HitCollisionEventTag, true);	
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_SpecialAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		PlayerCharacter->GetCombatComponent()->ClearOverlappingActors();
	}
	
	ResetComboCount();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_SpecialAttackBase::OnResetAttackCountGameplayEffectReceive(FGameplayEventData Payload)
{
	ResetComboCount();
}

TSubclassOf<UGameplayEffect> UTwoMinGA_SpecialAttackBase::GetAttackGameplayEffectClass() const
{
	return AttackGameplayEffectClass;
}

float UTwoMinGA_SpecialAttackBase::CalculationFightCost() const
{
	if (!FightCostCurveTable || FightCostName.IsNone()) return 0.f;

	int32 Level = GetAbilityLevel();
	const FString& LevelString = FString::FromInt(Level);
	const FRealCurve* Curve = FightCostCurveTable->FindCurve(FightCostName, LevelString);
	if (!Curve) return 0.f;
	
	const float FightCost = Curve->Eval(Level);
	if (FightCost <= 0.f) return 0.f;

	return FightCost;
}

bool UTwoMinGA_SpecialAttackBase::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const float CurrentFight = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentFightAttribute());
	if (!FightCostCurveTable || FightCostName.IsNone())
	{
		return true;	
	}
	
	return CurrentFight >= CalculationFightCost();
}

void UTwoMinGA_SpecialAttackBase::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	const float FightCost = CalculationFightCost();
	const UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (FightCost <= 0.f || !CostGE) return;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) return;

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(CostGE->GetClass(), GetAbilityLevel(),
		ASC->MakeEffectContext());
	
	if (!Spec.IsValid()) return;
	
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Cost_Fight, -FightCost);
	ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void UTwoMinGA_SpecialAttackBase::CustomEventReceived(FGameplayEventData Payload)
{
	Super::CustomEventReceived(Payload);
	
	if (HitCollisionMap.Contains(CurComboCount) == false) return;
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;
	
	EnableHitCollision(PlayerCharacter);
}

