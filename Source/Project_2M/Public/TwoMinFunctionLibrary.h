// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "TwoMinFunctionLibrary.generated.h"

struct FSaveGameData;
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
	static bool IsUsingGamePad(const UWorld* World, FPlatformUserId UserId);
	
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

	UFUNCTION()
	static bool IsVillageMap(const UWorld* World);
	
	UFUNCTION()
	static void SaveGame(const FSaveGameData& NewSaveGameData);
	
	UFUNCTION()
	static bool TryLoadGame(FSaveGameData& OutSaveGameData);
	
	UFUNCTION()
	static bool IsExistSaveGameData();
	
	UFUNCTION()
	static void RemoveSaveGameData();
	
	UFUNCTION()
	static bool IsLoadingData(const UObject* WorldContextObject);
	
	UFUNCTION()
	static void PlaySoundAtLocation(const UObject* WorldContextObject, USoundBase* PlaySound, const FVector& PlayLocation, 
		const float PlayVolume = 1);
};
