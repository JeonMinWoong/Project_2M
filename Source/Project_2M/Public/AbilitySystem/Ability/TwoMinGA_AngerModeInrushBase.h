// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_AngerModeInrushBase.generated.h"

class UNiagaraComponent;
class AHitCollisionBase;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_AngerModeInrushBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

	virtual void CustomEventReceived(FGameplayEventData Payload) override;
	virtual TSubclassOf<UGameplayEffect> GetAttackGameplayEffectClass() const override;
	
private:
	UFUNCTION()
	void AttachEffect(FGameplayEventData Payload);
	
	UPROPERTY(EditDefaultsOnly, Category = "AngerMode|Montage")
	UAnimMontage* AngerModeMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "AngerMode|AttachEffect")
	UNiagaraSystem* AngerModeEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "AngerMode|AttachEffectEventTag")
	FGameplayTag AttachEffectEventTag;
	
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> CachedAngerModeComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage|HitCollisionEventTag")
	FGameplayTag HitCollisionEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage|HitCollision")
	TSubclassOf<AHitCollisionBase> HitCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Damage|AttackInfoData")
	FAttackInfoData AttackInfosData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage|GameplayEffect")
	TSubclassOf<UGameplayEffect> AttackGameplayEffectClass;

public:
	FORCEINLINE FAttackInfoData& GetAttackInfoData() { return AttackInfosData; }
	FORCEINLINE TSubclassOf<AHitCollisionBase> GetHitCollisionBase() { return HitCollision; }
};
