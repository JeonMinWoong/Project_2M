// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_DeathBase.h"

#include "AbilitySystemGlobals.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/Weapon/TwoMinWeaponBase.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinGA_DeathBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	if (DeathAnimMontageMap.IsEmpty())
	{
		CustomCancelAbility();
		return;
	}
	
	const int32 MontageNumber = GetDeathMontageNumber();
	if (DeathAnimMontageMap.Contains(MontageNumber) == false)
	{
		CustomCancelAbility();
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* AnimTask = PlayToAnimMontage(DeathAnimMontageMap[MontageNumber]);
	if (AnimTask == nullptr)
	{
		CustomCancelAbility();
		return;
	}

	AnimTask->OnBlendOut.AddDynamic(this, &UTwoMinGA_DeathBase::CustomBlendOutAbility);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

int32 UTwoMinGA_DeathBase::GetDeathMontageNumber() const
{
	return UTwoMinFunctionLibrary::HasGameplayTag(
		GetAvatarActorFromActorInfo(),
		TwoMinGameplayTag::Shared_State_Throwing) ? 1 : 0;
}

void UTwoMinGA_DeathBase::CustomBlendOutAbility()
{
	if (ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		MyCharacter->GetMesh()->bPauseAnims = true;
		
		MyCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyCharacter->GetBaseUIComponent()->RemoveFromRoot();

		TArray<ATwoMinWeaponBase*> WeaponBases = MyCharacter->GetCombatComponent()->GetCharacterCurrentEquippedWeapon();
		for (ATwoMinWeaponBase* WeaponBase : WeaponBases)
		{
			if (WeaponBase)
			{
				if (RemoveDelay >= 0)
				{
					WeaponBase->SetLifeSpan(RemoveDelay);	
				}
			}
		}
		
		if (RemoveDelay >= 0)
		{
			MyCharacter->SetLifeSpan(RemoveDelay);	
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
