// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/Combat/BaseCombatComponent.h"

#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "Item/Weapon/TwoMinWeaponBase.h"

void UBaseCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, ATwoMinWeaponBase* InWeaponToRegister)
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister)) return;
	
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);
}

TArray<ATwoMinWeaponBase*> UBaseCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (CharacterCarriedWeaponMap.IsEmpty())
	{
		return TArray<ATwoMinWeaponBase*>();
	}
	
	TArray<ATwoMinWeaponBase*> WeaponArray;
	CharacterCarriedWeaponMap.GenerateValueArray(WeaponArray);
	return WeaponArray;
}

void UBaseCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
	}
	// Todo : Enemy
}

void UBaseCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
{
	TArray<ATwoMinWeaponBase*> WeaponToToggle = GetCharacterCurrentEquippedWeapon();

	if (WeaponToToggle.IsEmpty()) return;

	for (const ATwoMinWeaponBase* Weapon : WeaponToToggle)
	{
		if (!Weapon) continue;

		if (bShouldEnable)
		{
			Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OverlappingActors.Empty();
		}
	}
}

void UBaseCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	
}

void UBaseCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	
}
