// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TwoMinAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Interfaces/BaseUIInterface.h"

UTwoMinAttributeSet::UTwoMinAttributeSet()
{
	InitMaxLevel(30.f);
	InitCurrentLevel(1.f);
	
	InitMaxHealth(1.f);
	InitCurrentHealth(1.f);
	
	InitMaxStamina(1.f);
	InitCurrentStamina(1.f);
	
	InitMaxHealth(0.f);
	InitCurrentHealth(0.f);
	
	InitMaxExperience(1.f);
	InitCurrentExperience(0.f);
	
	InitAttackPower(1.f);
	InitDefensePower(1.f);
	
	InitDamageTo(1.f);

	InitGiveExperience(0);

	InitMaxGroggy(1.f);
	InitCurrentGroggy(0.f);
	InitGroggyTo(0.f);
	InitDecreaseGroggyDelay(1.f);
	
	InitGiveGold(0);
	InitCurrentGold(0);
	InitMaxGold(0);
}

void UTwoMinAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (!CachedBaseUInterface.IsValid())
	{
		CachedBaseUInterface = TWeakInterfacePtr<IBaseUIInterface>(Data.Target.GetAvatarActor());
		OwningBaseCharacter = Cast<ATwoMinBaseCharacter>(Data.Target.GetAvatarActor());
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
	
	if (Data.EvaluatedData.Attribute == GetCurrentFightAttribute())
	{
		const float NewCurrentFight = FMath::Clamp(GetCurrentFight(), 0.f, GetMaxFight());

		SetCurrentFight(NewCurrentFight);
		
		if (GetCurrentFight() == GetMaxFight())
		{
			UTwoMinFunctionLibrary::RemoveGameplayTagToActor(
				Data.Target.GetOwnerActor(),
				TwoMinGameplayTag::Player_State_ZeroFight
			);
			
			UTwoMinFunctionLibrary::AddGameplayTagToActor(
				Data.Target.GetOwnerActor(),
				TwoMinGameplayTag::Player_State_FullFight
			);
		}
		else
		{
			UTwoMinFunctionLibrary::RemoveGameplayTagToActor(
				Data.Target.GetOwnerActor(),
				TwoMinGameplayTag::Player_State_FullFight
			);
			
			if (UTwoMinFunctionLibrary::IsNearFloatZero(GetCurrentFight()))
			{
				UTwoMinFunctionLibrary::AddGameplayTagToActor(
					Data.Target.GetOwnerActor(),
					TwoMinGameplayTag::Player_State_ZeroFight
				);
			}
			else
			{
				UTwoMinFunctionLibrary::RemoveGameplayTagToActor(
					Data.Target.GetOwnerActor(),
				TwoMinGameplayTag::Player_State_ZeroFight
				);
			}
		}
		
		if (UPlayerUIComponent* PlayerUIComponent = CachedBaseUInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentFightChanged.Broadcast(GetCurrentFight()/GetMaxFight());	
		}
	}

	if (Data.EvaluatedData.Attribute == GetCurrentExperienceAttribute())
	{
		bool bIsLevelUp = false;
		float NewCurrentExperience = GetCurrentExperience();
		
		int32 NewCurrentLevel = GetCurrentLevel();
		int32 NewLevel = FMath::Clamp(NewCurrentLevel, 1, GetMaxLevel());
		
		ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(Data.Target.GetAvatarActor());
		if (PlayerCharacter)
		{
			while (NewLevel < GetMaxLevel() && NewCurrentExperience >= NeedToExperienceValue(PlayerCharacter, NewLevel))
			{
				NewCurrentExperience -= NeedToExperienceValue(PlayerCharacter, NewLevel);
				NewLevel++;
			}

			SetCurrentLevel(NewLevel);
			SetCurrentExperience(NewCurrentExperience);
			bIsLevelUp = NewLevel > NewCurrentLevel;
		}
		
		if (UPlayerUIComponent* PlayerUIComponent = CachedBaseUInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentExperienceChanged.Broadcast(GetCurrentExperience()/GetMaxExperience());
			PlayerUIComponent->OnCurrentLevelChanged.Broadcast(GetCurrentLevel());
		}

		if (bIsLevelUp)
		{
			PlayerCharacter->PlayerLevelUp(NewLevel);	
		}
	}

	if (Data.EvaluatedData.Attribute == GetGiveExperienceAttribute())
	{
		const float NewGiveExperience = GetGiveExperience();

		SetGiveExperience(NewGiveExperience);
	}

	if (Data.EvaluatedData.Attribute == GetCurrentGroggyAttribute())
	{
		const float NewCurrentGroggy = FMath::Clamp(GetCurrentGroggy(), 0.f, GetMaxGroggy());

		SetCurrentGroggy(NewCurrentGroggy);
	}
	
	if (Data.EvaluatedData.Attribute == GetCurrentGoldAttribute())
	{
		float NewCurrentGold = FMath::Clamp(GetCurrentGold(), 0.f, GetMaxGold());
		float GainGold = OwningBaseCharacter->GetGainGold();
		
		SetCurrentGold(NewCurrentGold);
		if (UPlayerUIComponent* PlayerUIComponent = CachedBaseUInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentGoldChanged.Broadcast(NewCurrentGold, GainGold);
		}
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

	if (Data.EvaluatedData.Attribute == GetGroggyToAttribute())
	{
		const float OldGroggy = GetCurrentGroggy();
		const float GroggyToValue = GetGroggyTo();

		const float NewCurrentGroggy = FMath::Clamp(OldGroggy + GroggyToValue, 0.f, GetMaxGroggy());

		SetCurrentGroggy(NewCurrentGroggy);

		if (GetCurrentGroggy() >= GetMaxGroggy())
		{
			AActor* Instigator = Data.EffectSpec.GetEffectContext().GetInstigator();
			if (!Instigator) return;

			FGameplayEventData Payload;
			Payload.Instigator = Instigator;
			Payload.Target = Data.Target.GetAvatarActor();
			
			UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
				Data.Target.GetAvatarActor(),
				TwoMinGameplayTag::Enemy_Event_Groggy,
				Payload
			);

			SetCurrentGroggy(0);
		}
	}
}

int32 UTwoMinAttributeSet::NeedToExperienceValue(ATwoMinPlayerCharacter* PlayerCharacter, int32 InCurrentLevel)
{
	UCurveTable* CurveTable = PlayerCharacter->GetNeedToLevelUp_ExperienceCurveTable();
	const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.MaxExperience"), TEXT("NeedToExperienceValue"));
	const int32 NeedToExperience = Curve->Eval(InCurrentLevel);
	return NeedToExperience;
}
