// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "TwoMinGA_ThrowBase.generated.h"

class ATwoMinProjectileBase;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_ThrowBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()
	
public:
	void DestroyProjectile();
	FRotator GetThrowRotator();
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	virtual TSubclassOf<UGameplayEffect> GetAttackGameplayEffectClass() const override;
	virtual void OnAttackGameplayEventReceivedByRange(FGameplayEventData Payload) override;
	
private:
	void CachedProjectileMesh();

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* ThrowAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage|Range")
	FGameplayTag SpawnProjectileEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Range")
	TSubclassOf<ATwoMinProjectileBase> Projectile;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Range")
	FName ShootSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage|AttackInfoData")
	FAttackInfoData AttackInfosData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage|GameplayEffect")
	TSubclassOf<UGameplayEffect> AttackGameplayEffectClass;
	
	UPROPERTY()
	UStaticMesh* NewStaticMesh;

public:
	FORCEINLINE FAttackInfoData& GetAttackInfoData() { return AttackInfosData; }
	virtual bool TryGetAttackInfoData(FAttackInfoData& OutData) const override { OutData = AttackInfosData; return true; }
	FORCEINLINE TSubclassOf<ATwoMinProjectileBase> GetProjectile() const { return Projectile; }
	FORCEINLINE FName GetShootSocketName() const { return ShootSocketName; }
	FORCEINLINE UStaticMesh* GetCustomStaticMesh() const { return NewStaticMesh; }
	
};
