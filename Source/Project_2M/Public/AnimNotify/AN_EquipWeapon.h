// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify/BaseAnimNotify.h"
#include "AN_EquipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UAN_EquipWeapon : public UBaseAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "UnequipSocket")
	FName UnequipSocket;
	
	UPROPERTY(EditAnywhere, Category = "EquipSocket")
	FName EquipSocketName;
	
protected:
	//~Begin UAnimNotify Interface.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	//~End UAnimNotify Interface.
	
};
