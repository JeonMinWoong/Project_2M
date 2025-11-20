
#pragma once

#include "GameplayTagContainer.h"
#include "TwoMinEnumTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

#include "TwoMinStructTypes.generated.h"

class ATargetPoint;
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
struct FExecutionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ExecutionTotalScore;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExecutionRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExecutionFrontAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExecutionBackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExecutionDistance;
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
struct FTwoMinEnemyAttackApproachData
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
struct FHitStopAttackerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackerDelay_Hit = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackerDelay_Guard = 0.0f;
};

USTRUCT(BlueprintType)
struct FHitStopVictimData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VictimDelay_Hit = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VictimDelay_Guard = 0.0f;
};

USTRUCT(BlueprintType)
struct FAttackInfoData : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

	FAttackInfoData() : AttackType(), AttackDamageCoef(), HitData(), HitDirectionNumber(0) {}
	
	FAttackInfoData(EAttackType InAttackType, float InAttackDamageCoef) : AttackType(InAttackType),
	 AttackDamageCoef(InAttackDamageCoef), HitData(), HitDirectionNumber(0) { }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttackType AttackType = EAttackType::Light;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDamageCoef;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="AttackType != EAttackType::Execution"))
	FHitData HitData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="AttackType != EAttackType::Execution"))
	FHitStopAttackerData HitStopAttackerData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="AttackType != EAttackType::Execution"))
	FHitStopVictimData HitStopVictimData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="AttackType != EAttackType::Execution"))
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
struct FProjectileInfoData : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

	FProjectileInfoData() : ProjectileHitPos() {}
	
	FProjectileInfoData(const FVector& NewHitPos) : ProjectileHitPos(NewHitPos) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector ProjectileHitPos;
};

UCLASS(BlueprintType)
class UProjectilePayloadObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	FProjectileInfoData Data;
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

USTRUCT(BlueprintType)
struct FEnemyAIPatrolData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ATargetPoint* PatrolPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WaitTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FEnemyAIAttackConditionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinAttackRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float TargetAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AbilityCooldownTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ShouldNotExistTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ShouldExistTag;
};


USTRUCT(BlueprintType)
struct FExecutionInfoData : public FGameplayAbilityTargetData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	TMap<int32, FAttackInfoData> AttackInfosData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	float SnapSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	float SnapMaxDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	float SnapDistance;
};

UCLASS(BlueprintType)
class UExecutionPayloadObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	bool bIsExecutionForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	int32 ExecutionNumber;
};