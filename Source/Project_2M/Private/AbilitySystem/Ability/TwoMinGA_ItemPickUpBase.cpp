// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_ItemPickUpBase.h"

#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Ability/Task/TwoMinAbilityTask.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UTwoMinGA_ItemPickUpBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		PlayerCharacter->GetPlayerUIComponent()->OnPossiblePickUpItem.Broadcast(true);
	}

	StartPickUpItem();
	WaitGameplayEvent(TriggerEventTag);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_ItemPickUpBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		PlayerCharacter->GetPlayerUIComponent()->OnPossiblePickUpItem.Broadcast(false);
	}
	
	if (CachedPickUpItem)
	{
		CachedPickUpItem->SetIsPickUpItem(false);
		CachedPickUpItem = nullptr;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_ItemPickUpBase::CustomCompleteAbility()
{
	bIsPickUpItemPlayer = false;
}

void UTwoMinGA_ItemPickUpBase::CustomOnBlendOutAbility()
{
	bIsPickUpItemPlayer = false;
}

void UTwoMinGA_ItemPickUpBase::StartPickUpItem()
{
	PickUpItemPlayer = UTwoMinAT_UpdatePickUpItem_Player::CreateTickTask(this);
	if (!PickUpItemPlayer)
	{
		CustomCancelAbility();
		return;
	}

	PickUpItemPlayer->OnPickUpItemTick.AddUniqueDynamic(this, &UTwoMinGA_ItemPickUpBase::UpdateRangePickUpItem);
	PickUpItemPlayer->ReadyForActivation();
}

void UTwoMinGA_ItemPickUpBase::UpdateRangePickUpItem(float DeltaTime)
{
	PickUpItemGroup.Empty();
	
	const TArray<AActor*> IgnoreActors;
	
	const FVector Center = GetAvatarActorFromActorInfo()->GetActorLocation();
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Center,
		PickUpRange,
		ObjectType,
		ATwoMinPickUpItemBase::StaticClass(),
		IgnoreActors,
		PickUpItemGroup
	);

	if (PickUpItemGroup.IsEmpty())
	{
		if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
		{
			PlayerCharacter->GetPlayerUIComponent()->OnPossiblePickUpItem.Broadcast(false);
		}
		
		if (bIsPickUpItemPlayer)
		{
			return;
		}
		
		CustomCancelAbility();
	}
	else
	{
		bool bIsPickUpItem = false;
		for (auto PickUpItem : PickUpItemGroup)
		{
			ATwoMinPickUpItemBase* PickUpItemBase = Cast<ATwoMinPickUpItemBase>(PickUpItem);
			if (!PickUpItemBase) continue;
			if (PickUpItemBase->IsPickUpItem() == false)
			{
				bIsPickUpItem = true;
				break;
			}
		}
		
		if (bIsPickUpItem == false)
		{
			if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo()))
			{
				PlayerCharacter->GetPlayerUIComponent()->OnPossiblePickUpItem.Broadcast(false);
			}
			
			return;
		}
	}
}

void UTwoMinGA_ItemPickUpBase::CustomEventReceived(FGameplayEventData Payload)
{
	PickUpTriggerEvent();
}

void UTwoMinGA_ItemPickUpBase::CompletePickUpItem(FGameplayEventData Payload)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;
	
	if (!CachedPickUpItem) return;
	
	CachedPickUpItem->GetUpItem(PlayerCharacter);
}

void UTwoMinGA_ItemPickUpBase::PickUpTriggerEvent()
{
	PlayToAnimMontage(PickUpItemMontage, NAME_None, false, true);
	CachedPickUpItem = TryGetPickUpItem();
	if (CachedPickUpItem == nullptr)
	{
		CustomCancelAbility();
		return;
	}
	
	bIsPickUpItemPlayer = true;
	CachedPickUpItem->SetIsPickUpItem(true);
	
	UAbilityTask_WaitGameplayEvent* CompleteEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, TwoMinGameplayTag::Player_Event_CompletePickUpItem, nullptr, 
		true, true);
	CompleteEventTask->EventReceived.AddDynamic(this, &ThisClass::CompletePickUpItem);
	CompleteEventTask->ReadyForActivation();
}

ATwoMinPickUpItemBase* UTwoMinGA_ItemPickUpBase::TryGetPickUpItem() const
{
	if (PickUpItemGroup.IsEmpty())
	{
		return nullptr;
	}

	const AActor* MyActor = GetAvatarActorFromActorInfo();
	const FVector Center = MyActor->GetActorLocation();
	
	float MinPickUpAngle = MAX_FLT;
	float MinPickUpRange = MAX_FLT;
	AActor* PickUpItem = nullptr;
	
	for (AActor* TargetItem : PickUpItemGroup)
	{
		FVector MyForward = MyActor->GetActorForwardVector();
		FVector TargetLocation = (TargetItem->GetActorLocation() - MyActor->GetActorLocation()).GetSafeNormal2D();
		const float CurAngleDeg = UKismetMathLibrary::DegAcos(FVector::DotProduct(MyForward, TargetLocation));
		if (MinPickUpAngle <= CurAngleDeg) continue;

		MinPickUpAngle = CurAngleDeg;
		PickUpItem = TargetItem;
		
		const float CurDistance = FVector::Distance(Center, TargetItem->GetActorLocation());
		if (MinPickUpRange <= CurDistance) continue;

		MinPickUpRange = CurDistance;
		PickUpItem = TargetItem;
	}
	
	return Cast<ATwoMinPickUpItemBase>(PickUpItem);
}


