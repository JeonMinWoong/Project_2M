// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "Item/PickUp/TwoMinPickUpItemBase.h"
#include "PlayerTask/TwoMinAT_UpdatePickUpItem_Player.h"
#include "TwoMinGA_ItemPickUpBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_ItemPickUpBase : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	virtual void CustomCompleteAbility() override;
	virtual void CustomOnBlendOutAbility() override;
	
	void StartPickUpItem();

	UFUNCTION()
	void UpdateRangePickUpItem(float DeltaTime);
	
	virtual void CustomEventReceived(FGameplayEventData Payload) override;
	
	UFUNCTION()
	void CompletePickUpItem(FGameplayEventData Payload);
	
	void PickUpTriggerEvent();
	ATwoMinPickUpItemBase* TryGetPickUpItem() const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="PickUpItem|Montage")
	UAnimMontage* PickUpItemMontage;

	UPROPERTY(EditDefaultsOnly, Category="PickUpItem|EventTag")
	FGameplayTag TriggerEventTag;

	UPROPERTY(EditDefaultsOnly, Category="PickUpItem|ObjectType")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	
	UPROPERTY(EditDefaultsOnly, Category="PickUpItem|Range")
	float PickUpRange;

	UPROPERTY()
	TArray<AActor*> PickUpItemGroup;

	UPROPERTY()
	UTwoMinAT_UpdatePickUpItem_Player* PickUpItemPlayer;
	
	UPROPERTY()
	ATwoMinPickUpItemBase* CachedPickUpItem;
	
	bool bIsPickUpItemPlayer = false;
};
