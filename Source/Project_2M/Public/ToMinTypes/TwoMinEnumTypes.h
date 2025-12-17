
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
	Loop,
	PingPong
};


UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Normal,
	Homing,
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