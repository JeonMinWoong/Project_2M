// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TwoMinAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "TwoMinDebugHelper.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Interfaces/BaseUIInterface.h"

UTwoMinAttributeSet::UTwoMinAttributeSet()
{
	InitMaxHealth(1.f);
	InitCurrentHealth(1.f);
	InitMaxStamina(1.f);
	InitCurrentStamina(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
	InitDamageTo(1.f);
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

		if (UPlayerUIComponent* PlayerUIComponent = CachedBaseUInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentStaminaChanged.Broadcast(GetCurrentStamina()/GetMaxStamina());	
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageToAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageToValue = GetDamageTo();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageToValue, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		BaseUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
		
		if (GetCurrentHealth() == 0.f)
		{
			TwoMinDebugHelper::Print(FString::Printf(TEXT("%s : Dead"), *GetOwningActor()->GetName()), FColor::Red);
		}
	}
}
