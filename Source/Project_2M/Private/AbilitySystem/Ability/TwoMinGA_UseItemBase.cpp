// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_UseItemBase.h"

#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Widgets/Player/TwoMinWidgetPlayer.h"

void UTwoMinGA_UseItemBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	if (UseItemMontages.IsEmpty())
	{
		CustomCancelAbility();
		return;
	}

	if (!TriggerEventData->OptionalObject->IsValidLowLevel())
	{
		CustomCancelAbility();
		return;
	}

	const UConsumePayloadObject* ConsumePayloadObject = Cast<UConsumePayloadObject>(TriggerEventData->OptionalObject);
	if (!ConsumePayloadObject)
	{
		CustomCancelAbility();
		return;
	}
	
	CachedConsumeData = ConsumePayloadObject->ConsumeData;
	int32 ConsumeIndex = static_cast<int32>(CachedConsumeData.ConsumeType);
	
	PlayToAnimMontage(UseItemMontages[ConsumeIndex]);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_UseItemBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_UseItemBase::ApplyItemConsumeEffect() const
{
	if (CachedConsumeData.ConsumePower.IsEmpty()) return;
	
	if (CachedConsumeData.ConsumeType == EConsumeType::Heal)
	{
		GetTwoMinAbilitySystemComponentFromActorInfo()->GiveHealthPercent(CachedConsumeData.ConsumePower[EStatusType::HpHeal]);
	}
	else
	{
		ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
		if (!PlayerCharacter) return;

		UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
		if (!ASC) return;
		
		UTwoMinWidgetPlayer* WidgetPlayer = Cast<UTwoMinWidgetPlayer>(PlayerCharacter->GetHUDOverlay());
		if (!WidgetPlayer) return;
		
		if (ASC->GetBuffItemEffectNum() > WidgetPlayer->GetMaxBuffCount()) return;
		
		ASC->AddConsumeBuff(CachedConsumeData.ItemDataBase.ItemID);
		PlayerCharacter->GetPlayerUIComponent()->OnSetBuffItem.Broadcast(CachedConsumeData.ItemDataBase.ItemID);
	}
}
