// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Compnents/BaseComponent.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "BaseCombatComponent.generated.h"

class UExecutionComponent;
class UAutoTargetingComponent;
class ATwoMinWeaponBase;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBaseCombatComponent : public UBaseComponent
{
	GENERATED_BODY()

public:
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, ATwoMinWeaponBase* InWeaponToRegister);
	void UnRegisterWeapon(FGameplayTag InWeaponTagToRegister);
	
	TArray<ATwoMinWeaponBase*> GetCharacterCurrentEquippedWeapon() const;
	
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType =
		EToggleDamageType::None);
	
	virtual UAutoTargetingComponent* GetAutoTargetingComponent() const;
	virtual UExecutionComponent* GetExecutionComponent() const;
	void ClearOverlappingActors();
	
	UFUNCTION(BlueprintCallable)
	void EnableWeaponsMesh(bool bIsEnable);
	
protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);

	virtual void OnHitTargetActor(AActor* HitActor, int32 HitWeaponIndex);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor, int32 HitWeaponIndex);

	UPROPERTY()
	TArray<AActor*> OverlappingActors;
	
private:

	UPROPERTY()
	TMap<FGameplayTag, ATwoMinWeaponBase*> CharacterCarriedWeaponMap;

	bool IsAlive = true;
	
public:
	FORCEINLINE bool GetIsAlive() const { return IsAlive; }
	FORCEINLINE void SetIsAlive(bool bInIsAlive) { IsAlive = bInIsAlive; }
};
