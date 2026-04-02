// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEnemyGameplayAbility.h"
#include "Item/Projectile/TwoMinProjectileBase.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinEGA_AttackBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinEGA_AttackBase : public UTwoMinEnemyGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void AddComboCount();
	
	UFUNCTION()
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

	UPROPERTY(EditDefaultsOnly, Category= "AttackTypeSetting")
	EAttackRangeType AttackRangeType = EAttackRangeType::Melee;

#pragma region Melee
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Melee")
	FGameplayTag OnHitEventTag;

#pragma endregion

#pragma region Range
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Range")
	FGameplayTag OnShootEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Range")
	TSubclassOf<ATwoMinProjectileBase> Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "Attack|Range")
	FName ShootSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Range")
	FGameplayTag OnTargetEventTag;
	
#pragma endregion
	
private:
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
	
	// Dummy 전용
	FVector StartLocation;

public:
	FORCEINLINE FAttackInfoData& GetAttackInfoData() { return AttackInfosData[CurComboCount]; }
	virtual bool TryGetAttackInfoData(FAttackInfoData& OutData) const override { if (auto* D = AttackInfosData.Find(CurComboCount)) { OutData = *D; return true; } return false; }
	FORCEINLINE TSubclassOf<ATwoMinProjectileBase> GetProjectile() const { return Projectile; }
	FORCEINLINE FName GetShootSocketName() const { return ShootSocketName; }

};
