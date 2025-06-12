// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TwoMinAbilitySystemComponent.generated.h"

struct FTwoMinPlayerAbilitySet;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);
	
	void GrantHeroWeaponAbilities(const TArray<FTwoMinPlayerAbilitySet>& InDefaultWeaponAbilities, int ApplyLevel,
		TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);
};
