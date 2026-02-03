// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_InteractionBase.h"

#include "AbilitySystem/Ability/PlayerTask/TwoMinAT_UpdateInteraction.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/InteractionActorBase.h"

void UTwoMinGA_InteractionBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		PlayerCharacter->GetPlayerUIComponent()->OnPossibleInteraction.Broadcast(true);
	}
	
	StartInteraction();
	WaitGameplayEvent(TriggerEventTag);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_InteractionBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		PlayerCharacter->GetPlayerUIComponent()->OnPossibleInteraction.Broadcast(false);
	}
	
	bIsInteracting = false;
	bIsPossibleInteraction = false;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_InteractionBase::StartInteraction()
{
	AT_Interaction = UTwoMinAT_UpdateInteraction::CreateTickTask(this);
	if (!AT_Interaction)
	{
		CustomCancelAbility();
		return;
	}

	AT_Interaction->OnInteractionTick.AddUniqueDynamic(this, &UTwoMinGA_InteractionBase::UpdateInteraction);
	AT_Interaction->ReadyForActivation();
}

void UTwoMinGA_InteractionBase::UpdateInteraction(float DeltaTime)
{
	InteractionGroup.Empty();
	
	const TArray<AActor*> IgnoreActors;
	
	const FVector Center = GetAvatarActorFromActorInfo()->GetActorLocation();
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Center,
		InteractionRange,
		ObjectType,
		AInteractionActorBase::StaticClass(),
		IgnoreActors,
		InteractionGroup
	);

	if (InteractionGroup.IsEmpty())
	{
		if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
		{
			PlayerCharacter->GetPlayerUIComponent()->OnPossibleInteraction.Broadcast(false);
		}
		
		CustomCancelAbility();
	}
	else
	{
		bIsPossibleInteraction = false;
		for (AActor* TargetActor : InteractionGroup)
		{
			AInteractionActorBase* InteractionActorBase = Cast<AInteractionActorBase>(TargetActor);
			if (!InteractionActorBase) continue;
			if (!InteractionActorBase->IsPossibleInteraction()) continue;
			
			bIsPossibleInteraction = true;
			break;
		}
		
		if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
		{
			PlayerCharacter->GetPlayerUIComponent()->OnPossibleInteraction.Broadcast(bIsPossibleInteraction);
		}
	}
}

void UTwoMinGA_InteractionBase::CustomEventReceived(FGameplayEventData Payload)
{
	if (bIsPossibleInteraction == false) return;
	
	AInteractionActorBase* InteractionActor = TryGetInteractionActor();
	if (!InteractionActor)
	{
		CustomCancelAbility();
		return;	
	}
	
	InteractionTriggerEvent(InteractionActor);
}

AInteractionActorBase* UTwoMinGA_InteractionBase::TryGetInteractionActor()
{
	if (InteractionGroup.IsEmpty()) return nullptr;
	
	const AActor* MyActor = GetAvatarActorFromActorInfo();
	const FVector MyLocation = MyActor->GetActorLocation();
	
	AInteractionActorBase* InteractionActor = nullptr;
	float MinPickUpRange = MAX_FLT;
	
	for (AActor* TargetActor : InteractionGroup)
	{
		AInteractionActorBase* InteractionActorBase = Cast<AInteractionActorBase>(TargetActor);
		if (!InteractionActorBase) continue;
		if (!InteractionActorBase->IsPossibleInteraction()) continue;
		
		const float CurDistance = FVector::Distance(MyLocation, InteractionActorBase->GetActorLocation());
		if (MinPickUpRange <= CurDistance) continue;

		MinPickUpRange = CurDistance;
		InteractionActor = InteractionActorBase;
	}
	
	return InteractionActor;
}

void UTwoMinGA_InteractionBase::InteractionTriggerEvent(AInteractionActorBase* InteractionActor)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;
	
	bIsInteracting = true;
	InteractionActor->Interact(PlayerCharacter);
}
