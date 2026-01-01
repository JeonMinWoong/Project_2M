// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGameplayAbility_WeaponSpawn.generated.h"

class ATwoMinWeaponBase;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGameplayAbility_WeaponSpawn : public UTwoMinGameplayAbility
{
	GENERATED_BODY()
	
public:
	void OnChangeWeapon(TSubclassOf<ATwoMinWeaponBase> InWeaponClass, bool bIsFirst);
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "WeaponClass")
	TSubclassOf<ATwoMinWeaponBase> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "AttachSocket")
	FName AttachSocketName = "";

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTag WeaponSpawnTag = FGameplayTag::EmptyTag;
	
};
