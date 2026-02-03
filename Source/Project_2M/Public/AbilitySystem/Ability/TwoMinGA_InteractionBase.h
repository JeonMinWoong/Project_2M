// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "TwoMinGA_InteractionBase.generated.h"

class AInteractionActorBase;
class UTwoMinAT_UpdateInteraction;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_InteractionBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface
	
	void StartInteraction();
	
	UFUNCTION()
	void UpdateInteraction(float DeltaTime);
	
	virtual void CustomEventReceived(FGameplayEventData Payload) override;

private:
	AInteractionActorBase* TryGetInteractionActor();
	void InteractionTriggerEvent(AInteractionActorBase* InteractionActor);
	
	UPROPERTY()
	UTwoMinAT_UpdateInteraction* AT_Interaction;
	
	UPROPERTY()
	TArray<AActor*> InteractionGroup;
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction|EventTag")
	FGameplayTag TriggerEventTag;

	UPROPERTY(EditDefaultsOnly, Category="Interaction|ObjectType")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction|Range")
	float InteractionRange;
	
	UPROPERTY()
	bool bIsInteracting;
	
	UPROPERTY()
	bool bIsPossibleInteraction;
	
};
