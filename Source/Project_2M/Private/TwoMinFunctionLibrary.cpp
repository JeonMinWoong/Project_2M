// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoMinFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"

bool UTwoMinFunctionLibrary::IsTargetPawnHostile(APawn* OwnerPawn, APawn* TargetPawn)
{
	IGenericTeamAgentInterface* OwnerTeamAgent = Cast<IGenericTeamAgentInterface>(OwnerPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (OwnerTeamAgent && TargetTeamAgent)
	{
		return OwnerTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	
	return false;
}

bool UTwoMinFunctionLibrary::HasGameplayTag(AActor* InActor, FGameplayTag InTag)
{
	if (!InActor) return false;

	UTwoMinAbilitySystemComponent* ASC =
		Cast<UTwoMinAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
	if (ASC)
	{
		return ASC->HasMatchingGameplayTag(InTag);
	}

	return false;
	
}
