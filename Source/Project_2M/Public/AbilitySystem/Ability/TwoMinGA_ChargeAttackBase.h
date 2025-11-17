// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "TwoMinGA_ChargeAttackBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_ChargeAttackBase : public UTwoMinGA_AttackBase
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void PreActivate(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
		const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	float CustomAddStaminaCost() const;

	float CustomCalculationStaminaCost() const;
	
	virtual void CustomApplyCost() const;
	
private:
	UFUNCTION()
	void OnChargeUp(FGameplayEventData Payload);

	UFUNCTION()
	void OnChargeEnd(FGameplayEventData Payload);

	bool IsFullCharge() const;

	UPROPERTY(EditDefaultsOnly, Category = "Charge|ReceiveTag")
	FGameplayTag  ChargeUpEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "Charge|ReceiveTag")
	FGameplayTag  ChargeEndEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Charge|MaxChargeCount")
	int32 MaxChargeCount;

	UPROPERTY(EditDefaultsOnly, Category = "Charge|CustomCostName")
	FName CustomCostName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Charge|AddStaminaCost")
	float FullChargeAddStaminaCostValue = 0.f;
	
	int32 CurChargeCount = 1;

public:
	FORCEINLINE virtual FAttackInfoData& GetAttackInfoData() override { return AttackInfosData[CurChargeCount]; }
	
};
