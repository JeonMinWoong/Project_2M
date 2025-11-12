// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_ExecutionBase.h"
#include "TwoMinGA_ExecutionTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_ExecutionTarget : public UTwoMinGA_ExecutionBase
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface
	
private:
	UAnimMontage* GetExecutionMontage(bool bIsExecutionForward, int32 ExecutionNumber);

	UFUNCTION()
	void OnExecutionDeathGameplayEventReceive(FGameplayEventData Payload);
	
	UPROPERTY(EditDefaultsOnly, Category = "ExecutionTarget|ExecutionDeathTag")
	FGameplayTag OnExecutionDeathTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "ExecutionTarget|Montages")
	TMap<int32, UAnimMontage*> ExecutionForwardTargetMontages;

	UPROPERTY(EditDefaultsOnly, Category = "ExecutionTarget|Montages")
	TMap<int32, UAnimMontage*> ExecutionBackTargetMontages;

	UPROPERTY()
	ATwoMinBaseCharacter* ExecutionCaster;
};
