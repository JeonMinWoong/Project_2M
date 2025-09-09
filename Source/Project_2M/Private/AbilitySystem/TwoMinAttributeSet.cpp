// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TwoMinAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Interfaces/BaseUIInterface.h"

UTwoMinAttributeSet::UTwoMinAttributeSet()
{
	InitMaxHealth(1.f);
	InitCurrentHealth(1.f);
	
	InitMaxStamina(1.f);
	InitCurrentStamina(1.f);
	
	InitMaxExperience(1.f);
	InitCurrentExperience(0.f);
	
	InitAttackPower(1.f);
	InitDefensePower(1.f);
	
	InitDamageTo(1.f);

	InitGiveExperience(1.f);
}

void UTwoMinAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (!CachedBaseUInterface.IsValid())
	{
		CachedBaseUInterface = TWeakInterfacePtr<IBaseUIInterface>(Data.Target.GetAvatarActor());
	}
	
	UBaseUIComponent* BaseUIComponent = CachedBaseUInterface->GetBaseUIComponent();
	
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		BaseUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
	}

	if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		const float NewCurrentStamina = FMath::Clamp(GetCurrentStamina(), 0.f, GetMaxStamina());

		SetCurrentStamina(NewCurrentStamina);

		if (GetCurrentStamina() == GetMaxStamina())
		{
			UTwoMinFunctionLibrary::AddGameplayTagToActor(
				Data.Target.GetOwnerActor(),
				TwoMinGameplayTag::Shared_State_FullStamina
			);
		}
		else
		{
			UTwoMinFunctionLibrary::RemoveGameplayTagToActor(
				Data.Target.GetOwnerActor(),
				TwoMinGameplayTag::Shared_State_FullStamina
			);
		}
		
		if (UPlayerUIComponent* PlayerUIComponent = CachedBaseUInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentStaminaChanged.Broadcast(GetCurrentStamina()/GetMaxStamina());	
		}
	}

	if (Data.EvaluatedData.Attribute == GetCurrentExperienceAttribute())
	{
		const float NewCurrentExperience = FMath::Clamp(GetCurrentExperience(), 0.f, GetMaxExperience());

		SetCurrentExperience(NewCurrentExperience);

		if (UPlayerUIComponent* PlayerUIComponent = CachedBaseUInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentExperienceChanged.Broadcast(GetCurrentExperience()/GetMaxExperience());	
		}
	}

	if (Data.EvaluatedData.Attribute == GetGiveExperienceAttribute())
	{
		const float NewGiveExperience = GetGiveExperience();

		SetGiveExperience(NewGiveExperience);
	}

	if (Data.EvaluatedData.Attribute == GetDamageToAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageToValue = GetDamageTo();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageToValue, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		BaseUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
		
		if (GetCurrentHealth() <= 0.f)
		{
			AActor* Instigator = Data.EffectSpec.GetEffectContext().GetInstigator();
			if (!Instigator) return;

			FGameplayEventData Payload;
			Payload.Instigator = Instigator;
			Payload.Target = Data.Target.GetAvatarActor();
			
			UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
				Data.Target.GetAvatarActor(),
				TwoMinGameplayTag::Shared_Event_Death,
				Payload
			);
		}
	}
}
