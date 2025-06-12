
#pragma once

#include "GameplayTagContainer.h"

#include "TwoMinStructTypes.generated.h"

class UTwoMinGameplayAbility;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FTwoMinPlayerAbilitySet
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTwoMinGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FTwoMinPlayerWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* WeaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	TArray<FTwoMinPlayerAbilitySet> DefaultWeaponAbilities;
};