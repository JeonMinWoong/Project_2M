// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTo.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAttributeSet.h"

struct FTwoMinDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTo)

	FTwoMinDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTwoMinAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTwoMinAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTwoMinAttributeSet, DamageTo, Target, false);
	}
};

static FTwoMinDamageCapture GetTwoMinDamageCapture()
{
	static FTwoMinDamageCapture TwoMinDamageCapture;
	return TwoMinDamageCapture;
}

UGEExecCalc_DamageTo::UGEExecCalc_DamageTo()
{
	RelevantAttributesToCapture.Add(GetTwoMinDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetTwoMinDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetTwoMinDamageCapture().DamageToDef);

	SourceUnBreakAttackTag = TwoMinGameplayTag::Shared_State_UnBreakAttack;
	TargetGuardStateTag = TwoMinGameplayTag::Shared_State_Guarding;
	TargetInvincibleStateTag = TwoMinGameplayTag::Shared_State_Invincible;
}

void UGEExecCalc_DamageTo::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
 	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	bool bIsUnBreakAttack = HasStateTag(ExecutionParams.GetSourceAbilitySystemComponent(), SourceUnBreakAttackTag);
	bool bIsInvincible = HasStateTag(ExecutionParams.GetTargetAbilitySystemComponent(), TargetInvincibleStateTag);
	bool bIsGuard = HasStateTag(ExecutionParams.GetTargetAbilitySystemComponent(), TargetGuardStateTag);
	
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetTwoMinDamageCapture().AttackPowerDef,
		EvaluateParameters,
		 SourceAttackPower
	);

	float BeforeAttackDamageCoef = 0.f;
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(TwoMinGameplayTag::Shared_SetByCaller_BaseDamage))
		{
			BeforeAttackDamageCoef = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(TwoMinGameplayTag::Shared_SetByCaller_GaurdSuccess))
		{
			bIsGuard = TagMagnitude.Value > 0 ? bIsGuard : false; 
		}
	}

	bool bIsNoneDamage = bIsInvincible ? true : bIsGuard && bIsUnBreakAttack == false;
	
	SourceAttackPower = bIsNoneDamage ? 0.f : SourceAttackPower;
	float AttackDamageCoef = bIsNoneDamage ? 0 : BeforeAttackDamageCoef;
	
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetTwoMinDamageCapture().DefensePowerDef,
		EvaluateParameters,
		TargetDefensePower
	);

	float TargetDamageTaken = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetTwoMinDamageCapture().DamageToDef,
		EvaluateParameters,
		TargetDamageTaken
	);
	

	const float TotalAttackDamage = SourceAttackPower * AttackDamageCoef;
	float TargetDefenseCorrection = SourceAttackPower / (SourceAttackPower + TargetDefensePower) * 100;
	TargetDefenseCorrection = FMath::RoundToInt32(TargetDefenseCorrection) * 0.01f;
	
	int32 FinalDamageDone = FMath::RoundToInt32(TotalAttackDamage * TargetDefenseCorrection);
	if (FinalDamageDone < 1)
	{
		FinalDamageDone = 1;
	}
	
	OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetTwoMinDamageCapture().DamageToProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
}

bool UGEExecCalc_DamageTo::HasStateTag(const UAbilitySystemComponent* TargetASC,
	const FGameplayTag GameplayTag) const
{
	if (!TargetASC) return false;

	if (TargetASC->HasMatchingGameplayTag(GameplayTag))
	{
		return true;
	}

	return false;
}
