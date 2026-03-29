// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_EquipWeapon.h"

#include "TwoMinGameplayTag.h"

void UTwoMinGA_EquipWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter)
	{
		CustomCancelAbility();
		return;
	}
	
	bool NextEquipWeapon = static_cast<bool>(TriggerEventData->EventMagnitude);
	bool CurEquipWeapon = PlayerCharacter->GetEquipWeapon();
	if (NextEquipWeapon == CurEquipWeapon)
	{
		CustomCancelAbility();
		return;
	}
	
	if (NextEquipWeapon)
	{
		if (!EquipAnimMontage)
		{
			CustomCancelAbility();
			return;
		}
	
		PlayToAnimMontage(EquipAnimMontage);
		PlayerCharacter->SetEquipWeapon(true);
		UTwoMinFunctionLibrary::AddGameplayTagToActor(PlayerCharacter, TwoMinGameplayTag::Player_State_EquipWeapon);
	}
	else
	{
		if (!UnequipAnimMontage)
		{
			CustomCancelAbility();
			return;
		}
	
		PlayToAnimMontage(UnequipAnimMontage);
		PlayerCharacter->SetEquipWeapon(false);
		UTwoMinFunctionLibrary::RemoveGameplayTagToActor(PlayerCharacter, TwoMinGameplayTag::Player_State_EquipWeapon);
	}
}
