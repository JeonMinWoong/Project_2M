// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEnemyGameplayAbility.h"
#include "TwoMinEGA_GroggyBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinEGA_GroggyBase : public UTwoMinEnemyGameplayAbility
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
	
private:
	void StartOutline(const ATwoMinEnemyCharacter* EnemyCharacter);
	
	UFUNCTION()
	void FadeOutOutline();
	
	void FinishOutline(const ATwoMinEnemyCharacter* EnemyCharacter);

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* GroggyAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Outline|FinishOutlineEventTag")
	FGameplayTag FinishOutlineEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Outline|Material")
	UMaterialInterface* OutlineMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category = "Outline|Thickness")
	float ThicknessValue;
	
	UPROPERTY(EditDefaultsOnly, Category = "Outline|LineColor")
	FVector4 LineColor;
	
	UPROPERTY()
	UMaterialInstanceDynamic* OutlineDynamicMaterial;
	
	FTimerHandle OutlineFadeOutTimerHandle;
};
