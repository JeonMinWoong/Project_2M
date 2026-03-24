// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_PerfectGuardBase.h"
#include "TwoMinGA_PerfectGuard_Player.generated.h"

class UTwoMinGA_Guard_Player;
class UInputAction;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_PerfectGuard_Player : public UTwoMinGA_PerfectGuardBase
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface
	
private:
	UFUNCTION()
	void FadeOutOutline();
	
	void StartOutline(const ATwoMinPlayerCharacter* PlayerCharacter);
	void FinishOutline();
	
	UPROPERTY(EditAnywhere, Category = "CheckInputAction")
	TObjectPtr<const UInputAction> InputActionToCheck;

	UPROPERTY(EditAnywhere, Category = "CheckAbility")
	TSubclassOf<UGameplayAbility> GuardAbilityClass;
	
	UPROPERTY(EditAnywhere, Category = "Outline|Material")
	UMaterialInterface* OutlineMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Outline|Thickness")
	float ThicknessValue;
	
	UPROPERTY()
	UMaterialInstanceDynamic* OutlineDynamicMaterial;
	
	FTimerHandle OutlineFadeOutTimerHandle;
};
