// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "Item/HitBox/HitCollisionBase.h"
#include "TwoMinGA_SpecialAttackBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_SpecialAttackBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()
	
public:
	void AddComboCount();
	void ResetComboCount();

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.
	
	virtual void OnResetAttackCountGameplayEffectReceive(FGameplayEventData Payload) override;
	
	virtual TSubclassOf<UGameplayEffect> GetAttackGameplayEffectClass() const override;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
						   const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	virtual void CustomEventReceived(FGameplayEventData Payload) override;
	
private:
	float CalculationFightCost() const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|HitEventTag")
	FGameplayTag OnHitEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Attack|AttackInfoData")
	TMap<int, FAttackInfoData> AttackInfosData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Combo")
	int32 MaxComboCount = 1;
	
	UPROPERTY()
	int32 CurComboCount = 1;

	UPROPERTY(EditAnywhere, Category = "Attack|GameplayEffect")
	TSubclassOf<UGameplayEffect> AttackGameplayEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category= "FightCost")
	UCurveTable* FightCostCurveTable;

	UPROPERTY(EditDefaultsOnly, Category= "FightCostName")
	FName FightCostName;
	
	UPROPERTY(EditDefaultsOnly, Category = "HitCollision|HitEventTag")
	FGameplayTag HitCollisionEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "HitCollision|Map")
	TMap<int, TSubclassOf<AHitCollisionBase>> HitCollisionMap;
	
public:
	FORCEINLINE virtual FAttackInfoData& GetAttackInfoData() { return AttackInfosData[CurComboCount]; }
	virtual bool TryGetAttackInfoData(FAttackInfoData& OutData) const override { if (auto* D = AttackInfosData.Find(CurComboCount)) { OutData = *D; return true; } return false; }
	FORCEINLINE TSubclassOf<AHitCollisionBase> GetHitCollisionBase() { return HitCollisionMap[CurComboCount]; }
};
