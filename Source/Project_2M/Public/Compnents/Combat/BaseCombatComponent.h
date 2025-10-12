// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Compnents/BaseComponent.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "BaseCombatComponent.generated.h"

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
	
	TArray<ATwoMinWeaponBase*> GetCharacterCurrentEquippedWeapon() const;
	
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType =
		EToggleDamageType::None);
	
	virtual UAutoTargetingComponent* GetAutoTargetingComponent() const;

	void ClearOverlappingActors();
	
protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);

	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

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
