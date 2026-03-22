// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_InteractionBase.h"

#include "AbilitySystem/Ability/PlayerTask/TwoMinAT_UpdateInteraction.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/InteractionActorBase.h"
#include "System/InteractionActor_NPC.h"

void UTwoMinGA_InteractionBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
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
		PlayerCharacter->GetPlayerUIComponent()->OnPossibleInteraction.Broadcast(EInteractionType::None, false);
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
			PlayerCharacter->GetPlayerUIComponent()->OnPossibleInteraction.Broadcast(EInteractionType::None, false);
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
			CachedInteractionActor = InteractionActorBase;
			break;
		}
		
		if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
		{
			EInteractionType InteractionType = CachedInteractionActor->GetInteractionType();
			if (InteractionType == EInteractionType::None) return;
			
			if (InteractionType == EInteractionType::MapSelect)
			{
				PlayerCharacter->GetPlayerUIComponent()->OnPossibleInteraction.Broadcast(EInteractionType::NPC, false);
				PlayerCharacter->GetPlayerUIComponent()->OnPossibleInteraction.Broadcast(InteractionType, bIsPossibleInteraction);
			}
			else
			{
				PlayerCharacter->GetPlayerUIComponent()->OnPossibleInteraction.Broadcast(EInteractionType::MapSelect, false);
				
				if (CachedInteractionActor->IsHiddenCondition())
				{
					bIsPossibleInteraction = false;
				}
				
				PlayerCharacter->GetPlayerUIComponent()->OnPossibleInteraction.Broadcast(InteractionType, bIsPossibleInteraction);
			}
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
	
	if (Cast<AInteractionActor_NPC>(InteractionActor))
	{
		const UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetWorld()->GetGameInstance());
		if (!GI) return;
	
		GI->PlayUISound(EUISoundType::Focus_Select);	
	}
}
