// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoMinFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Character/TwoMinBaseCharacter.h"

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

void UTwoMinFunctionLibrary::SendToGameplayEffectEvent(AActor* InActor, FGameplayTag InEventTag,
	const FGameplayEventData& InEventData)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		InActor,
		InEventTag,
		InEventData
	);
}

void UTwoMinFunctionLibrary::AddGameplayTagToActor(AActor* InActor, FGameplayTag TagToAdd)
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(InActor);
	if (!MyCharacter) return;
	UTwoMinAbilitySystemComponent* Asc = MyCharacter->GetAbilitySystemComponent();
	if (!Asc) return;
	
	if (!Asc->HasMatchingGameplayTag(TagToAdd))
	{
		Asc->AddLooseGameplayTag(TagToAdd);
	}
}

void UTwoMinFunctionLibrary::RemoveGameplayTagToActor(AActor* InActor, FGameplayTag TagToAdd)
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(InActor);
	if (!MyCharacter) return;
	UTwoMinAbilitySystemComponent* Asc = MyCharacter->GetAbilitySystemComponent();
	if (!Asc) return;
	
	if (Asc->HasMatchingGameplayTag(TagToAdd))
	{
		Asc->RemoveLooseGameplayTag(TagToAdd);
	}
}

float UTwoMinFunctionLibrary::AttackTypeChangeToAmount(EAttackType AttackType)
{
	switch (AttackType)
	{
	case EAttackType::Light:
		return 10;
	case EAttackType::Heavy:
		return 20;
	default:
		return 0;
	}
}

EItemType UTwoMinFunctionLibrary::GetItemType(int32 ItemID)
{
	int32 Value = ItemID;

	while (Value >= 10)
	{
		Value /= 10;
	}
	
	if (Value < static_cast<int8>(EItemType::Consume)) return EItemType::Equipment;
	if (Value < static_cast<int8>(EItemType::Etc)) return EItemType::Consume;
	if (Value < static_cast<int8>(EItemType::Unknown)) return EItemType::Etc;

	return EItemType::None;
}

constexpr float EPS = 0.0001f;
bool UTwoMinFunctionLibrary::IsNearFloatZero(float Value)
{
	return FMath::IsNearlyZero(Value, EPS);
}

bool UTwoMinFunctionLibrary::IsNearFloatEqual(float Value, float EqualValue)
{
	return FMath::IsNearlyEqual(Value, EqualValue);
}