// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_EquipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_EquipWeapon : public UTwoMinGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* EquipAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* UnequipAnimMontage;
};
