// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_SendGameplayEventToSelf.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Character/TwoMinBaseCharacter.h"

void UAN_SendGameplayEventToSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         const FAnimNotifyEventReference& EventReference)
{
	ATwoMinBaseCharacter* BaseCharacter = Cast<ATwoMinBaseCharacter>(MeshComp->GetOwner());
	if (!BaseCharacter) return;

	FGameplayEventData EventData;
	EventData.Instigator = MeshComp->GetOwner();
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		BaseCharacter,
		EventTag,
		EventData
	);
	
	Super::Notify(MeshComp, Animation, EventReference);
}
