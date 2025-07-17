
#pragma once

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