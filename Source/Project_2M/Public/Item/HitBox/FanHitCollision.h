// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HitBox/HitCollisionBase.h"
#include "FanHitCollision.generated.h"

enum class EFanRotationType : uint8;
/**
 * 
 */

// 사용 하지 않음.
UCLASS()
class PROJECT_2M_API AFanHitCollision : public AHitCollisionBase
{
	GENERATED_BODY()
	
public:
	AFanHitCollision();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	void SweepBetweenValue(float FromValue, float ToValue);
	FRotator SweepRotation(float InValue) const;
	
	UPROPERTY(EditAnywhere)
	EFanRotationType FanRotationType;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsReverse;
	
	UPROPERTY(EditAnywhere)
	float StartValue;        // 공격 시작 각
	
	UPROPERTY(EditAnywhere)
	float EndValue;          // 공격 종료 각
	
	UPROPERTY(EditAnywhere)
	FVector BoxExtent;
	
	UPROPERTY(EditAnywhere)
	float Radius = 200.f;  // 부채꼴 반지름
	
	UPROPERTY(EditAnywhere)
	float StepAngle = 5.f; // 각도 분할 (3~7 추천)
	
	UPROPERTY(EditAnywhere)
	float RotateSpeed = 1.f;
	
	UPROPERTY(VisibleAnywhere)
	float CurrentValue;
	
	UPROPERTY(VisibleAnywhere)
	float PreviousValue;
};
