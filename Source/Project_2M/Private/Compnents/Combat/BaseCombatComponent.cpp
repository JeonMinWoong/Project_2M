// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/Combat/BaseCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Item/Weapon/TwoMinWeaponBase.h"

void UBaseCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, ATwoMinWeaponBase* InWeaponToRegister)
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister)) return;
	
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);
}

void UBaseCombatComponent::UnRegisterWeapon(FGameplayTag InWeaponTagToRegister)
{
	if (!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister)) return;
	
	ATwoMinWeaponBase* WeaponBase = CharacterCarriedWeaponMap[InWeaponTagToRegister];
	WeaponBase->OnWeaponHitTarget.Unbind();
	WeaponBase->OnWeaponPulledFromTarget.Unbind();
	CharacterCarriedWeaponMap.Remove(InWeaponTagToRegister);
	
	WeaponBase->Destroy();
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
	if (ToggleDamageType == EToggleDamageType::None)
	{
		return;
	}

	if (ToggleDamageType <= EToggleDamageType::CurrentEquippedWeapon_All)
	{
		ToggleCurrentEquippedWeaponCollision(bShouldEnable, ToggleDamageType);
	}

	// Todo : Enemy
}

UAutoTargetingComponent* UBaseCombatComponent::GetAutoTargetingComponent() const
{
	return nullptr;
}

UExecutionComponent* UBaseCombatComponent::GetExecutionComponent() const
{
	return nullptr;
}

void UBaseCombatComponent::ClearOverlappingActors()
{
	OverlappingActors.Empty();
}

void UBaseCombatComponent::EnableWeaponsMesh(bool bIsEnable)
{
	for (const auto CarriedWeaponMap : CharacterCarriedWeaponMap)
	{
		CarriedWeaponMap.Value->EnableMesh(bIsEnable);
	}
}

void UBaseCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	TArray<ATwoMinWeaponBase*> WeaponToToggle = GetCharacterCurrentEquippedWeapon();

	if (WeaponToToggle.IsEmpty()) return;

	for (const ATwoMinWeaponBase* Weapon : WeaponToToggle)
	{
		if (!Weapon) continue;

		if (Weapon->ToggleDamageType != EToggleDamageType::CurrentEquippedWeapon_All)
		{
			if (Weapon->ToggleDamageType != ToggleDamageType)
			{
				continue;
			}
		}
		
		if (bShouldEnable)
		{
			Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void UBaseCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	ATwoMinBaseCharacter* Character = Cast<ATwoMinBaseCharacter>(HitActor);
	if (!Character) return;
	if (Character->GetCombatComponent()->GetIsAlive() == false) return;
	if (OverlappingActors.Contains(HitActor)) return;

	OverlappingActors.AddUnique(HitActor);

	FGameplayEventData Data;
	
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		TwoMinGameplayTag::Shared_Event_MeleeHit,
		Data
	);
}

void UBaseCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	
}