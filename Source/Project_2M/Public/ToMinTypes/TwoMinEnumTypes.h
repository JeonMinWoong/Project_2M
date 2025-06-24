
#pragma once

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	None = 0,
	CurrentEquippedWeapon_Left,
	CurrentEquippedWeapon_Right,
	CurrentEquippedWeapon_All,
};