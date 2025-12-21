// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_AngerModeStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_AngerModeStateBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.
	
private:
	UFUNCTION()
	void OnEndAngerMode();
	
	void ApplyAngerBuff(UTwoMinAbilitySystemComponent* ASC);
	void RemoveAngerBuff(UTwoMinAbilitySystemComponent* ASC);
	
	void OnSetFightBarUI(const ATwoMinPlayerCharacter* PlayerCharacter, const bool bIsOnAngerMode) const;
	
	void StartDecreaseAngerEffect(UTwoMinAbilitySystemComponent* ASC, const ATwoMinPlayerCharacter* PlayerCharacter);
	void EndDecreaseAngerEffect(UTwoMinAbilitySystemComponent* ASC);
	
	UPROPERTY(EditDefaultsOnly, Category = "EndTag")
	FGameplayTag AngerModeEndTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Buffs")
	TArray<FAngerBuffData> AngerBuffData;
};
