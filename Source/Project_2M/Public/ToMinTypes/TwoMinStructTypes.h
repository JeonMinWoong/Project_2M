
#pragma once

#include "GameplayTagContainer.h"
#include "TwoMinEnumTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

#include "TwoMinStructTypes.generated.h"

class ULevelSequence;
class ATwoMinEnterEventBase;
class ATwoMinEnemyCharacter;
class ATwoMinWeaponBase;
class UImage;
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
	UInputMappingContext* WeaponInputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<FTwoMinPlayerAbilitySet> DefaultWeaponAbilities;
};

USTRUCT(BlueprintType)
struct FTwoMinPlayerAutoTargetingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int TargetingTotalScore = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingRotationSpeed = 0.f;
};

USTRUCT(BlueprintType)
struct FExecutionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ExecutionTotalScore = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExecutionRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExecutionFrontAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExecutionBackAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExecutionDistance = 0.f;
};

USTRUCT(BlueprintType)
struct FTwoMinPlayerAttackApproachData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReachDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ApproachSpeed = 0.f;
};

USTRUCT(BlueprintType)
struct FTwoMinEnemyAttackApproachData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReachDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ApproachSpeed = 0.f;
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
	UCurveFloat* KnockBackCurve = nullptr;
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
	UCurveFloat* KnockBackCurve = nullptr;
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
	float AttackDamageCoef = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="AttackType != EAttackType::Execution"))
	FHitData HitData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="AttackType != EAttackType::Execution"))
	FHitStopAttackerData HitStopAttackerData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="AttackType != EAttackType::Execution"))
	FHitStopVictimData HitStopVictimData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="AttackType != EAttackType::Execution"))
	int HitDirectionNumber = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECameraShakeType CameraShakeType = ECameraShakeType::None;

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
	ATargetPoint* PatrolPoint = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WaitTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FEnemyAIAttackConditionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinAttackRange = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAttackRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float TargetAngle = 0.f;

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
	float SnapSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	float SnapMaxDuration = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	float SnapDistance = 0.f;
};

UCLASS(BlueprintType)
class UExecutionPayloadObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	bool bIsExecutionForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	int32 ExecutionNumber = 0;
};

USTRUCT(BlueprintType)
struct FItemDropData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemDrop|DropOwner")
	FName ItemDropOwner;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemDrop|ID")
	int32 ItemID = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemDrop|Weight")
	float Weight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemDrop|MinDropRange")
	int32 MinDropRange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemDrop|MaxCount")
	int32 MaxDropRangeCount = 0;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FItemData|ID")
	int32 ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FItemData|Name")
	FString ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FItemData|Type")
	EItemType ItemType = EItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FItemData|CurrentCount")
	int32 CurrentCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FItemData|MaxCount")
	int32 MaxCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FItemData|SellPrice")
	int32 SellPrice = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FItemData|BuyPrice")
	int32 BuyPrice = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemDrop|ItemTexture")
	UTexture2D* ItemTexture = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemDrop|ItemInformation")
	FString ItemInformation;
};

USTRUCT(BlueprintType)
struct FItemEquipmentData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData|Base")
	FItemData ItemDataBase;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData|Equipment|ET")
	EEquipmentType EquipmentType = EEquipmentType::Weapon_Left;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData|Equipment|Power")
	TMap<EStatusType, int32> EquipmentPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData|Equipment|Class")
	TSubclassOf<ATwoMinWeaponBase> WeaponClass;
};

USTRUCT(BlueprintType)
struct FItemConsumeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData|Base")
	FItemData ItemDataBase;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData|Consume|ConsumeType")
	EConsumeType ConsumeType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData|Consume|CP")
	TMap<EStatusType, float> ConsumePower;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData|Consume|Duration", meta=(EditCondition="ConsumeType == EConsumeType::Buff"))
	float BuffDuration = 0.f;
};

USTRUCT(BlueprintType)
struct FItemEtcData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData|Base")
	FItemData ItemDataBase;
	
};

USTRUCT(BlueprintType)
struct FItemInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemID")
	int32 ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HoldCount")
	int32 HoldCount = 0;
	
	UPROPERTY(BlueprintReadWrite)
	UTexture2D* ItemTexture = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsRegister = false;
	
	UPROPERTY(BlueprintReadWrite)
	EInventorySlotType SlotType = EInventorySlotType::Inventory;
	
	UPROPERTY(BlueprintReadWrite)
	int32 RegisterCount = -1;
	
	void OnRegister(bool NewIsRegister, EInventorySlotType NewSlotType, int32 NewRegisterCount)
	{
		this->bIsRegister = NewIsRegister;
		this->SlotType = NewSlotType;
		this->RegisterCount = NewRegisterCount;
	}
	
	void UnRegister()
	{
		this->bIsRegister = false;
		this->SlotType = EInventorySlotType::Inventory;
		this->RegisterCount = -1;
	}
	
};

USTRUCT(BlueprintType)
struct FItemPickUpEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 ItemID = 0;
	
	UPROPERTY(BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(BlueprintReadWrite)
	int32 ItemCount = 0;
	
	UPROPERTY(BlueprintReadWrite)
	UTexture2D* ItemTexture = nullptr;
};

UCLASS(BlueprintType)
class UConsumePayloadObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Payload")
	FItemConsumeData ConsumeData;
};

USTRUCT(BlueprintType)
struct FAngerBuffData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag BuffTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsPercent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BuffAmount = 0.f;
};

USTRUCT(BlueprintType)
struct FEnemyAIEvasionConditionDate
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PossibleMinAngle = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PossibleMaxAngle = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PossibleMinDistance = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PossibleMaxDistance = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AbilityCooldownTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ShouldNotExistTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ShouldExistTag;
};

USTRUCT(BlueprintType)
struct FTeleportData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TeleportStartDelay = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TeleportFinishDelay = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TeleportDistance = 0.f;
	
};

USTRUCT(BlueprintType)
struct FWorldStageNameData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString RealStageName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ShowStateName;
	
};

USTRUCT(BlueprintType)
struct FWorldStageClearGainData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ClearItemOwner;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GainGold = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GainExp = 0;
	
};

USTRUCT(NotBlueprintType)
struct FSaveGameData
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<FString, bool> WorldStageMap;
	
	UPROPERTY()
	int32 PlayerLevel = 0;
	
	UPROPERTY()
	int32 PlayerCurrentExp = 0;
	
	UPROPERTY()
	int32 PlayerCurrentGold = 0;
	
	UPROPERTY()
	TArray<FItemInstance> PlayerCurrentItems;
};

USTRUCT(BlueprintType)
struct FCinematicCharacterData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsHide = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString SyncCharacterName;
};

USTRUCT(BlueprintType)
struct FCharacterConversionData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATwoMinEnemyCharacter> PhaseTargetCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATwoMinEnterEventBase> PhaseLevelSequence;
};

USTRUCT(BlueprintType)
struct FPhaseConversionData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EBossPhaseType, FCharacterConversionData> BossPhaseType;
};

USTRUCT(BlueprintType)
struct FExecutionCinematicData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ULevelSequence* CinematicLevelSequence = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OriginCamConvertBlendDelay = 0.f;
};

USTRUCT(BlueprintType)
struct FInteractionTextData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> InteractionTextGroup;
};

USTRUCT(BlueprintType)
struct FNPCData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NPC_ID = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int32, FInteractionTextData> NPC_InteractionMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ArraySizeEnum="15"))
	TSet<int32> NPC_StoreItemIds;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EStoreDealTextType, FString> NPC_StoreDealTextGroup;
};

USTRUCT(BlueprintType)
struct FRichWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass;
};
