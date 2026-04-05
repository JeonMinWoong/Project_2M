// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HitBox/HitCollisionBase.h"
#include "CapsuleHitCollision.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API ACapsuleHitCollision : public AHitCollisionBase
{
	GENERATED_BODY()
	
public:
	ACapsuleHitCollision();
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void OnCheckHitActorCollision() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Radius")
	float Radius = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Height")
	float Height = 0.f;
	
};
