// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinGA_AttackBase.generated.h"

struct FTwoMinPlayerAutoTargetingData;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_AttackBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

public:
	UTwoMinGA_AttackBase();
	
	UFUNCTION()
	void AddComboCount();
	
	UFUNCTION()
	void ResetComboCount();
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

	virtual bool bIsReTriggerSameAbility() const override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Montages")
	TMap<int, UAnimMontage*> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|Combo")
	int32 MaxComboCount = 1;
 
	UPROPERTY()
	int32 CurComboCount = 1;
};
