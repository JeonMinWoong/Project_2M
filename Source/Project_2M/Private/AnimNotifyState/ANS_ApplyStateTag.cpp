// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_ApplyStateTag.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "TwoMinDebugHelper.h"

void UANS_ApplyStateTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (ASC && StateTagToApply.IsValid())
	{
		ASC->AddLooseGameplayTag(StateTagToApply);
	}

	FString Value = FString::Printf(TEXT("Add State Tag : %s"), *StateTagToApply.ToString());
	DebugTwoMin::Print(Value, FColor::Orange);
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_ApplyStateTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (ASC && StateTagToApply.IsValid())
	{
		ASC->RemoveLooseGameplayTag(StateTagToApply);
	}

	FString Value = FString::Printf(TEXT("End State Tag : %s"), *StateTagToApply.ToString());
	DebugTwoMin::Print(Value, FColor::Orange);
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
