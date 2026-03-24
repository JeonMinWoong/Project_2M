// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_AngerModeStateBase.h"

#include "NiagaraComponent.h"
#include "TwoMinDebugHelper.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Compnents/UI/PlayerUIComponent.h"

void UTwoMinGA_AngerModeStateBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	
	UAbilityTask_WaitGameplayTagAdded* WaitTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(
		this,
		AngerModeEndTag,
		nullptr
	);
	
	WaitTask->Added.AddUniqueDynamic(this, &UTwoMinGA_AngerModeStateBase::OnEndAngerMode);
	WaitTask->ReadyForActivation();
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	UTwoMinAbilitySystemComponent* ASC = Cast<UTwoMinAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	
	InitNiagaraComp(TriggerEventData);
	ApplyAngerBuff(ASC);
	OnSetFightBarUI(PlayerCharacter, true);
	StartDecreaseAngerEffect(ASC, PlayerCharacter);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_AngerModeStateBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	UTwoMinAbilitySystemComponent* ASC = Cast<UTwoMinAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	
	RemoveNiagaraComp();
	RemoveAngerBuff(ASC);
	EndDecreaseAngerEffect(ASC);
	OnSetFightBarUI(PlayerCharacter, false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_AngerModeStateBase::OnEndAngerMode()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTwoMinGA_AngerModeStateBase::InitNiagaraComp(const FGameplayEventData* TriggerEventData)
{
	UObject* NiagaraObj = const_cast<UObject*>(TriggerEventData->OptionalObject.Get());
	if (!NiagaraObj) return;
	
	CachedAngerModeComp = Cast<UNiagaraComponent>(NiagaraObj);
}

void UTwoMinGA_AngerModeStateBase::ApplyAngerBuff(UTwoMinAbilitySystemComponent* ASC)
{
	for (const FAngerBuffData& BuffData : AngerBuffData)
	{
		ASC->AddAngerBuffEffect(BuffData.BuffTag, BuffData.bIsPercent, BuffData.BuffAmount);
	}
}

void UTwoMinGA_AngerModeStateBase::RemoveAngerBuff(UTwoMinAbilitySystemComponent* ASC)
{
	for (const FAngerBuffData& BuffData : AngerBuffData)
	{
		ASC->RemoveAngerBuffEffect(BuffData.BuffTag);
	}
}

void UTwoMinGA_AngerModeStateBase::OnSetFightBarUI(const ATwoMinPlayerCharacter* PlayerCharacter, 
	const bool bIsOnAngerMode) const
{
	PlayerCharacter->GetPlayerUIComponent()->OnSetAngerState.Broadcast(bIsOnAngerMode);
}

void UTwoMinGA_AngerModeStateBase::RemoveNiagaraComp()
{
	if (!CachedAngerModeComp) return;
	
	CachedAngerModeComp->Rename(nullptr, GetWorld());

	CachedAngerModeComp->SetAutoDestroy(true);
	CachedAngerModeComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	CachedAngerModeComp->Deactivate();
	CachedAngerModeComp = nullptr;
}

void UTwoMinGA_AngerModeStateBase::StartDecreaseAngerEffect(UTwoMinAbilitySystemComponent* ASC,
                                                            const ATwoMinPlayerCharacter* PlayerCharacter)
{
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(
		PlayerCharacter->GetAngerDecreaseEffect()->GetClass(), 
		1.f, 
		ASC->MakeEffectContext()
	);
			
	ASC->SetAngerDecreaseHandle(ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get()));
}

void UTwoMinGA_AngerModeStateBase::EndDecreaseAngerEffect(UTwoMinAbilitySystemComponent* ASC)
{
	FActiveGameplayEffectHandle Handle = ASC->GetAngerDecreaseHandle();
	if (Handle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(Handle);
		ASC->ClearAngerDecreaseHandle();
	}
}
