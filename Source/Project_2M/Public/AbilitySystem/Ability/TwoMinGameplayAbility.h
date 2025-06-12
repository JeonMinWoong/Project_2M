// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TwoMinGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EToMinAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven,
};

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	void PlayToAnimMontage(UAnimMontage* AnimMontage);
	
	UPROPERTY(EditDefaultsOnly, Category = "AbilityPoicy")
	EToMinAbilityActivationPolicy AbilityActivationPolicy = EToMinAbilityActivationPolicy::OnTriggered;

private:
	UFUNCTION()
	void CustomEndAbility();
};
