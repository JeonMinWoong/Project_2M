// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TwoMinAbilitySystemComponent.generated.h"

class ATwoMinEnemyCharacter;
class UTwoMinGameplayAbility;
struct FTwoMinPlayerAbilitySet;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool CancelAndStartNextNewAction(const FGameplayTag& InInputTag);
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	void OnCancelAndReTriggerAbility(UTwoMinGameplayAbility* InAbilityCDO,
		const TSubclassOf<UTwoMinGameplayAbility> InAbilityToReTrigger, const FGameplayTag& AbilityTag);
	
	void GrantHeroWeaponAbilities(const TArray<FTwoMinPlayerAbilitySet>& InDefaultWeaponAbilities, int ApplyLevel,
		TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UTwoMinGameplayAbility* GetPlayingAbility();
	bool IsPlayingAbility(const FGameplayTag& AbilityTag);
	
	UTwoMinGameplayAbility* GetPlayingAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetActiveAbilityTag(FGameplayTagContainer GameplayTagContainer) const;
	UTwoMinGameplayAbility* GetActiveAbilityInputTag(const FGameplayTag& InInputTag);
	
	UTwoMinGameplayAbility* GetActiveAbility(const FGameplayTag& AbilityTag);

	// Enemy 전용
	UFUNCTION(BlueprintCallable, Category = "Enemy|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);


	// Player 전용
	UFUNCTION()
	void GiveExperience(ATwoMinEnemyCharacter* InEnemyCharacter);
	
	// Player 전용
	UFUNCTION()
	void GiveGold(ATwoMinEnemyCharacter* InEnemyCharacter);
	
	// Player 전용
	UFUNCTION()
	void GiveHealthPercent(float InHealthPercent);
};
