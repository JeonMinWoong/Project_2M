// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CircleHitCollision.h"
#include "SectorHitCollision.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API ASectorHitCollision : public ACircleHitCollision
{
	GENERATED_BODY()
	
public:
	ASectorHitCollision();
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool IsCustomHitCondition(AActor* HitActor) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "SectorAngle")
	float SectorAngle = 0.f;
};
