// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_AngerModeInrushBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "TwoMinConstant.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Item/HitBox/HitCollisionBase.h"

class UAbilityTask_WaitGameplayEvent;

void UTwoMinGA_AngerModeInrushBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	PlayToAnimMontage(AngerModeMontage);
	WaitGameplayEvent(HitCollisionEventTag, false);	
	
	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, AttachEffectEventTag, nullptr, true, true);

	EventTask->EventReceived.AddDynamic(this, &ThisClass::AttachEffect);
	EventTask->ReadyForActivation();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_AngerModeInrushBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	FGameplayEventData EventData;
	EventData.Instigator = GetAvatarActorFromActorInfo();
	EventData.Target = GetAvatarActorFromActorInfo();
	EventData.OptionalObject = CachedAngerModeComp;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetAvatarActorFromActorInfo(),
		TwoMinGameplayTag::Player_Event_AngerMode_Start,
		EventData
	);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_AngerModeInrushBase::CustomEventReceived(FGameplayEventData Payload)
{
	Super::CustomEventReceived(Payload);
	
	if (!HitCollision) return;
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;
	
	AHitCollisionBase* HitCollisionClass = HitCollision->GetDefaultObject<AHitCollisionBase>();
	if (!HitCollisionClass) return;
	
	EnableHitCollision(PlayerCharacter);
}

TSubclassOf<UGameplayEffect> UTwoMinGA_AngerModeInrushBase::GetAttackGameplayEffectClass() const
{
	return AttackGameplayEffectClass;
}

void UTwoMinGA_AngerModeInrushBase::AttachEffect(FGameplayEventData Payload)
{
	if (!AngerModeEffect) return;
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;
	
	CachedAngerModeComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		AngerModeEffect,
		PlayerCharacter->GetMesh(),
		TwoMinConstant::Player_Center,       
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		false
		);
}
