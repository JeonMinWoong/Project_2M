
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
	Ungaurdable
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
