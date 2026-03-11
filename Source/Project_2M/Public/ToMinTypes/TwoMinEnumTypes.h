
#pragma once

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	None = 0,
	Player,
	Enemy,
};

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	None = 0,
	CurrentEquippedWeapon_Left,
	CurrentEquippedWeapon_Right,
	CurrentEquippedWeapon_All,
};

UENUM(BlueprintType)
enum class ERotateDirectionType : uint8
{
	InputDirection = 0,
	TargetDirection,
};

UENUM(BlueprintType)
enum class ETranslationDistanceType : uint8
{
	InputDistance = 0,	
	TargetDistance,
};

UENUM(BlueprintType)
enum class EHeavyAttackComboType : uint8
{
	None = 0,
	MustBeLinkedToLightAttack,
};

UENUM(BlueprintType)
enum class EHitType : uint8
{
	Normal = 0,
	Push,
	Down,
	Throw
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None = 0,
	Light,
	Heavy,
	Ungaurdable,
	Execution,
};

UENUM(BlueprintType)
enum class EPatrolPathMode : uint8
{
	None,
	Loop,
	PingPong
};


UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Normal,
	Homing,
	Location,
};

UENUM(BlueprintType)
enum class EProjectilePierceType : uint8
{
	NonPiercing,
};

UENUM(BlueprintType)
enum class EAttackRangeType : uint8
{
	Melee,
	Range
};

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Practice,
	Normal,
	Elite,
	Boss,
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0,
	Equipment,
	Consume,
	Etc,
	Unknown,
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Weapon_Right,
	Weapon_Left,
	Weapon_Both,
};

UENUM(Blueprintable)
enum class EConsumeType : uint8
{
	Heal = 0,
	Buff = 1,
};

UENUM(BlueprintType)
enum class EInventorySlotType : uint8
{
	Equipment,
	Quick,
	Inventory,
};

UENUM(Blueprintable)
enum class EInventoryWindowType : uint8
{
	Equipment,
	Quick,
	Inventory,
};

UENUM(Blueprintable)
enum class EInventorySelectType : uint8
{
	InventoryToEquipment,
	InventoryToConsume,
	InventoryToEtc,
	InventoryToEquipped,
	QuickToConsume,
	EquipToEquipment,
};

UENUM(Blueprintable)
enum class ESelectEventType : uint8
{
	InventoryToConsume_Register,
	InventoryToConsume_Use,
	InventoryToEquipment_Change,
	QuickToConsume_NonRegister,
	QuickToConsume_Use,
	Cancel,
	OpenItemPopup,
	CloseItemPopup,
};

UENUM(Blueprintable)
enum class EStatusType : uint8
{
	None,
	HpHeal,
	Attack,
	Defense,
	MaxHealth,
	MaxStamina,
};

UENUM(Blueprintable)
enum class EAddMappingContextType : uint8
{
	None = 0,
	Normal = 1,
	FullFight = 2,
	AngerMode = 3,
};

UENUM(Blueprintable)
enum class EFanRotationType : uint8
{
	Horizontal = 0,
	Vertical = 1,
};

UENUM(Blueprintable)
enum class EHitCollisionType : uint8
{
	None = 0,
	Circle = 1,
	Box = 2,
	Sector = 3,
};

UENUM(Blueprintable)
enum class EHitCollisionSpawnType : uint8
{
	MyLocation = 0,
	TargetLocation = 1,
};

UENUM(Blueprintable)
enum class EEnemyEvasionType : uint8
{
	None = 0,
	Back = 1,
	Left = 2,
	Right = 3,
	All = 4,
};

UENUM(Blueprintable)
enum class EEnemyAIMoveType : uint8
{
	None = 0,
	Melee = 1,
	Middle = 2,
	Ranged = 3,
};

UENUM(Blueprintable)
enum class ECameraShakeType : uint8
{
	None = 0,
	Light = 1,
	Medium = 2,
	Heavy = 3,
};

UENUM(Blueprintable)
enum class ESpawnMonsterType : uint8
{
	None = 0,
	Skeleton_Sword_Man = 1,
	Skeleton_Axe = 2,
	Skeleton_Archer = 3,
	Skeleton_Mage = 4,
	
	Elite_Skeleton_Lord = 101,
	
	Boss_Necromancer = 201,
};

UENUM(Blueprintable)
enum class ESpawnMonsterLevel : uint8
{
	None = 0,
	Develop = 1,
	
	Level_1_1 = 2,
	Level_1_2 = 3,
	Level_1_3 = 4,
};

UENUM(Blueprintable)
enum class EDefeatType : uint8
{
	None = 0,
	Return = 1,
	Retry = 2,
};

UENUM(Blueprintable)
enum class EBossPhaseType : uint8
{
	Phase_Finish = 0,
	Phase_1 = 1,			// 추가 페이즈가 1 있음.
	Phase_2 = 2,
};

inline EBossPhaseType& operator--(EBossPhaseType& Phase)
{
	int Val = static_cast<int>(Phase);
	if (Val > 0) {
		Phase = static_cast<EBossPhaseType>(Val - 1);
	}
	return Phase;
}

UENUM(Blueprintable)
enum class EInteractionType : uint8
{
	None = 0,
	MapSelect = 1,
	NPC = 2,
};

UENUM(Blueprintable)
enum class ENPCType : uint8
{
	None = 0,
	Store = 1,
};