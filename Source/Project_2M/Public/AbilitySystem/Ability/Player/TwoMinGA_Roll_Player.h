// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_Roll_Player.generated.h"

class ATwoMinPlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_Roll_Player : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

public:
	UTwoMinGA_Roll_Player();
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	virtual bool bIsReTriggerSameAbility() const override;
	
	void StartRoll(const FGameplayAbilityActorInfo* ActorInfo);
	void NormalRoll();
	void LockRoll(ATwoMinPlayerCharacter* PlayerCharacter);

	void EndRoll(const FGameplayAbilityActorInfo* ActorInfo);
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Montages")
	UAnimMontage* RollMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|LockMontages")
	TMap<int, UAnimMontage*> LockRollMontages;
};
