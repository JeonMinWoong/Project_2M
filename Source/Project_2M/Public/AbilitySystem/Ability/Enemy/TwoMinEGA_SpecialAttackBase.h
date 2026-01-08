// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEnemyGameplayAbility.h"
#include "AbilitySystem/Ability/Task/TwoMinAT_TeleportTaskBase.h"
#include "TwoMinEGA_SpecialAttackBase.generated.h"

class AHitCollisionBase;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinEGA_SpecialAttackBase : public UTwoMinEnemyGameplayAbility
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
	
	virtual void OnAttackGameplayEventReceivedByLocation(FGameplayEventData Payload) override;
	
	virtual void CustomEventReceived(FGameplayEventData Payload) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Attack|HitEventTag")
	FGameplayTag OnHitEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack|LocationEventTag")
	FGameplayTag OnLocationEventTag;
	
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
	
	UPROPERTY(EditDefaultsOnly, Category = "HitCollision|HitEventTag")
	FGameplayTag HitCollisionEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "HitCollision|Map")
	TMap<int, TSubclassOf<AHitCollisionBase>> HitCollisionMap;
	
	UPROPERTY()
	FVector CachedTargetLocation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effect|CachedLocation")
	UNiagaraSystem* CachedLocationEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Additional|Task|Teleport")
	TSubclassOf<UTwoMinAT_TeleportTaskBase> TeleportTaskClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Additional|Task|Teleport", 
		meta = (EditCondition = "TeleportTaskClass != nullptr"))
	float TeleportStartDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Additional|Task|Teleport", 
		meta = (EditCondition = "TeleportTaskClass != nullptr"))
	float TeleportFinishDelay;
	
public:
	FORCEINLINE virtual FAttackInfoData& GetAttackInfoData() { return AttackInfosData[CurComboCount]; }
	FORCEINLINE TSubclassOf<AHitCollisionBase> GetHitCollisionBase() { return HitCollisionMap[CurComboCount]; }
	
	FORCEINLINE void SetCachedTargetLocation(const FVector& InLocation) { CachedTargetLocation = InLocation; }
};
