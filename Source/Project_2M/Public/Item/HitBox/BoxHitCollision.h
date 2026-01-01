// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HitBox/HitCollisionBase.h"
#include "BoxHitCollision.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API ABoxHitCollision : public AHitCollisionBase
{
	GENERATED_BODY()
	
public:
	ABoxHitCollision();
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void BeginPlay() override;
	
	virtual void OnCheckHitActorCollision() override;
	virtual void OnDamageToHitActor(AActor* HitActor) override;
	
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "BoxSize")
	FVector BoxExtent;
};
