// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Compnents/BaseComponent.h"
#include "BaseCombatComponent.generated.h"

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
};

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
		EToggleDamageType::CurrentEquippedWeapon);

protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);

	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);
	
private:
	UPROPERTY()
	TArray<AActor*> OverlappingActors;

	UPROPERTY()
	TMap<FGameplayTag, ATwoMinWeaponBase*> CharacterCarriedWeaponMap;
};
