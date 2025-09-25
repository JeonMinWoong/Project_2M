
#pragma once

#include "GameplayTagContainer.h"
#include "TwoMinEnumTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

#include "TwoMinStructTypes.generated.h"

enum class EHitType : uint8;
class UTwoMinPlayerLinkedAnimLayer;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UTwoMinPlayerLinkedAnimLayer> WeaponAnimLayerToLink;
	
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* WeaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	TArray<FTwoMinPlayerAbilitySet> DefaultWeaponAbilities;
};

USTRUCT(BlueprintType)
struct FTwoMinPlayerAutoTargetingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int TargetingTotalScore;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingRotationSpeed;
};

USTRUCT(BlueprintType)
struct FTwoMinPlayerAttackApproachData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReachDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ApproachSpeed;
};

USTRUCT(BlueprintType)
struct FHitData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EHitType HitType = EHitType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PushDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PushTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* KnockBackCurve;
};

USTRUCT(BlueprintType)
struct FGuardHitData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PushDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PushTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* KnockBackCurve;
};

USTRUCT(BlueprintType)
struct FAttackInfoData : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttackType AttackType = EAttackType::Light;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDamageCoef;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHitData HitData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int HitDirectionNumber;

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
};

UCLASS(BlueprintType)
class UAttackPayloadObject : public UObject
{
    GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
    FAttackInfoData Data;
};

USTRUCT(BlueprintType)
struct FEnemyAIData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetectRadius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PushTime = 0.0f;
};