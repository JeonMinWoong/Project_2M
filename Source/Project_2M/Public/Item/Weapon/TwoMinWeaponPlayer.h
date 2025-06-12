// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/TwoMinWeaponBase.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinWeaponPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinWeaponPlayer : public ATwoMinWeaponBase
{
	GENERATED_BODY()

public:
	virtual FTwoMinPlayerWeaponData GetWeaponData() const override;
	
	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	FTwoMinPlayerWeaponData PlayerWeaponData;
};
