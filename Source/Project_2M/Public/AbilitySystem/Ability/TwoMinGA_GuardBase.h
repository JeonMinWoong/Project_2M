// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_GuardBase.generated.h"

class ATwoMinBaseCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_GuardBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

public:
	virtual bool IsPossibleMustBeHoldAbilityImmediatelyCancel() const;

	bool IsGuardCondition(const ATwoMinBaseCharacter* Attacker, const ATwoMinBaseCharacter* Target) const;
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	virtual void CustomEventReceived(FGameplayEventData Payload) override;
	
	virtual void CustomCompleteAbility() override;
	
	virtual void CustomInterruptedAbility() override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	UFUNCTION()
	void OnHitGuard(FGameplayEventData Payload);

	UFUNCTION()
	void OnHitEnd();
	
	UPROPERTY(EditDefaultsOnly, Category = "Guard|Montages")
	UAnimMontage* GuardAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Guard|Montages")
	TMap<int, UAnimMontage*> HitGuardAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Guard|ReceiveTag")
	FGameplayTag  BeforeGuardEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "Guard|ReceiveTag")
	FGameplayTag  HitGuardEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Guard|Angle", meta=(ClampMin="0", ClampMax="180", UIMin="0", UIMax="90"))
	float GuardAngle = 45.f;

	UPROPERTY()
	bool bIsGuard = false;

	UPROPERTY()
	bool bIsHitGuard = false;

	UPROPERTY()
	bool bIsBreakGuard = false;
	
public:
	FORCEINLINE float GetGuardAngle() const { return GuardAngle; }
};

