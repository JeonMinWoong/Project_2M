// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "TwoMinFunctionLibrary.generated.h"

struct FGameplayEventData;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static bool IsTargetPawnHostile(APawn* OwnerPawn, APawn* TargetPawn);

	UFUNCTION()
	static bool HasGameplayTag(AActor* InActor, FGameplayTag InTag);

	UFUNCTION()
	static void SendToGameplayEffectEvent(AActor* InActor, FGameplayTag InEventTag,
		const FGameplayEventData& InEventData);

	UFUNCTION()
	static void AddGameplayTagToActor(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION()
	static void RemoveGameplayTagToActor(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION()
	static float AttackTypeChangeToAmount(EAttackType AttackType);
	
	UFUNCTION()
	static EItemType GetItemType(int32 ItemID);
	
	UFUNCTION()
	static bool IsNearFloatZero(float Value);
	
	UFUNCTION()
	static bool IsNearFloatEqual(float Value, float EqualValue);
};
