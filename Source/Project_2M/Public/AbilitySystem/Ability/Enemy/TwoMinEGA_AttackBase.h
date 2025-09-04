// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEnemyGameplayAbility.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinEGA_AttackBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinEGA_AttackBase : public UTwoMinEnemyGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

	virtual TSubclassOf<UGameplayEffect> GetAttackGameplayEffectClass() const override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|HitEventTag")
	FGameplayTag OnHitEventTag;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Attack|AttackInfoData")
	FAttackInfoData AttackInfoData;

	UPROPERTY(EditAnywhere, Category = "Attack|GameplayEffect")
	TSubclassOf<UGameplayEffect> AttackGameplayEffectClass;
	
	// Dummy 전용
	FVector StartLocation;

public:
	FORCEINLINE FAttackInfoData& GetAttackInfoData() { return AttackInfoData; }
};
