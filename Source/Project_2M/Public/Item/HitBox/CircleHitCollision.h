// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HitBox/HitCollisionBase.h"
#include "CircleHitCollision.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API ACircleHitCollision : public AHitCollisionBase
{
	GENERATED_BODY()
	
public:
	ACircleHitCollision();
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void OnCheckHitActorCollision() override;

	UPROPERTY(EditDefaultsOnly, Category = "Radius")
	float Radius = 0.f;
};
