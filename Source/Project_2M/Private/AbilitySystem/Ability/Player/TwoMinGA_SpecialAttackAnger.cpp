// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_SpecialAttackAnger.h"

#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAttributeSet.h"

void UTwoMinGA_SpecialAttackAnger::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              bool bReplicateEndAbility, bool bWasCancelled)
{
	UTwoMinAbilitySystemComponent* ASC = GetTwoMinAbilitySystemComponentFromActorInfo();
	for (auto AbilitySpec : ASC->GetActivatableAbilities())
	{
		if (!AbilitySpec.Ability->AbilityTags.HasTagExact(CancelAngerModeTag)) continue;
		
		if (!AbilitySpec.IsActive()) continue;
		
		ASC->CancelAbilityHandle(AbilitySpec.Handle);
		break;
	}
	
	AllFightConsumeToZero(ASC);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_SpecialAttackAnger::AllFightConsumeToZero(UTwoMinAbilitySystemComponent* ASC) const
{
	const float FightCost = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentFightAttribute());
	const UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (FightCost <= 0.f || !CostGE) return;

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(
		CostGE->GetClass(), GetAbilityLevel(),
		ASC->MakeEffectContext()
	);
	
	if (!Spec.IsValid()) return;
	
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Cost_Fight, -FightCost);
	ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}