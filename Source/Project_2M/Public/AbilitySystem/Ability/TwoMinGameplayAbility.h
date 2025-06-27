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

UENUM(BlueprintType)
enum class ETwoAbilityInputType : uint8
{
	Only,
	ReTriggerable,
};


/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UTwoMinGameplayAbility();
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	virtual bool bIsReTriggerSameAbility() const;
	
	void PlayToAnimMontage(UAnimMontage* AnimMontage);
	void CustomCancelAbility();
	
	UPROPERTY(EditDefaultsOnly, Category = "AbilityPoicy")
	EToMinAbilityActivationPolicy AbilityActivationPolicy = EToMinAbilityActivationPolicy::OnTriggered;

	UPROPERTY()
	ETwoAbilityInputType AbilityInputType = ETwoAbilityInputType::Only;

	UPROPERTY()
	bool bIsReTriggerAble = false;
	
private:
	UFUNCTION()
	void CustomEndAbility();

public:
	FORCEINLINE ETwoAbilityInputType GetAbilityInputType() const { return AbilityInputType; }
	FORCEINLINE bool IsReTriggerActive() const { return bIsReTriggerAble; }
};
