// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HitBox/FanHitCollision.h"

#include "Character/TwoMinBaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

AFanHitCollision::AFanHitCollision()
{
	
}

void AFanHitCollision::BeginPlay()
{
	Super::BeginPlay();

	PreviousValue = StartValue;
	CurrentValue  = StartValue;

	bIsReverse = StartValue > EndValue;
	OwnerCharacter = Cast<ATwoMinBaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void AFanHitCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PreviousValue = CurrentValue;
	
	CurrentValue += bIsReverse ? -(RotateSpeed * DeltaTime) : (RotateSpeed * DeltaTime);

	SweepBetweenValue(PreviousValue, CurrentValue);

	if (bIsReverse)
	{
		if (CurrentValue <= EndValue)
		{
			Destroy();
		}
	}
	else
	{
		if (CurrentValue >= EndValue)
		{
			Destroy();
		}	
	}
}

void AFanHitCollision::SweepBetweenValue(float FromValue, float ToValue)
{
	float DeltaValue = ToValue - FromValue;
	int32 Steps = FMath::CeilToInt(FMath::Abs(DeltaValue) / StepAngle);

	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	FRotator BaseRot = OwnerCharacter->GetActorRotation();
	
	for (int32 i = 0; i <= Steps; ++i)
	{
		float Alpha = (float)i / Steps;
		float LerpValue = FMath::Lerp(FromValue, ToValue, Alpha);

		FRotator Rot = BaseRot + FRotator(LerpValue, 0.f, 0.f);
		FVector Direction = Rot.Vector();
		FVector Center = OwnerLocation + Direction * Radius;
		
		TArray<FHitResult> HitResults;
			
		// UKismetSystemLibrary::BoxTraceMulti(
		// 	GetWorld(),
		// 	Center,
		// 	Center,
		// 	BoxExtent,
		// 	Rot,
		// 	TraceChannel,
		// 	false,
		// 	IgnoreActors,
		// 	EDrawDebugTrace::ForDuration,
		// 	HitResults,
		// 	true
		// );
	}
}

FRotator AFanHitCollision::SweepRotation(float InValue) const
{
	if (FanRotationType == EFanRotationType::Horizontal)
	{
		return FRotator(0.f, InValue, 0.f);
	}
	
	return FRotator(InValue, 0.f, 0.f);
}
