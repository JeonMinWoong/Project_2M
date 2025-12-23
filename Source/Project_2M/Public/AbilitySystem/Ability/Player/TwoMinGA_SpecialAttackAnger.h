// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Player/TwoMinGA_SpecialAttackBase.h"
#include "TwoMinGA_SpecialAttackAnger.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_SpecialAttackAnger : public UTwoMinGA_SpecialAttackBase
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

	void AllFightConsumeToZero(UTwoMinAbilitySystemComponent* ASC) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "InputCancelTag")
	FGameplayTag CancelAngerModeTag;
};
