// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HitBox/CircleHitCollision.h"

#include "TwoMinFunctionLibrary.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

ACircleHitCollision::ACircleHitCollision()
{
	HitCollisionType = EHitCollisionType::Circle;
}

void ACircleHitCollision::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (bDrawEditorDebug)
	{
#if WITH_EDITOR
		
		FQuat Rotation = GetActorQuat();
		FVector Center = GetActorLocation() + Rotation.RotateVector(Offset);

		DrawDebugSphere(
			GetWorld(),
			Center,
			Radius,
			12,
			FColor::Green,
			false,
			3.f,
			0,
			2.f
		);
#endif	
	}
}

void ACircleHitCollision::OnCheckHitActorCollision()
{
	Super::OnCheckHitActorCollision();
	
	TArray<FHitResult> HitResult;
	FRotator Rotation = GetActorRotation();
	FVector Center = GetActorLocation() + Rotation.RotateVector(Offset);
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		Center,
		Center,
		Radius,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	
	for (FHitResult& HitResultValue : HitResult)
	{
		AActor* HitActor = HitResultValue.GetActor();
		if (!HitActor) continue;
		
		ATwoMinBaseCharacter* TargetCharacter = Cast<ATwoMinBaseCharacter>(HitActor);
		if (!TargetCharacter) continue;
		
		if (UTwoMinFunctionLibrary::IsTargetPawnHostile(OwnerCharacter, TargetCharacter) == false) continue;
		
		if (IgnoreActors.Contains(TargetCharacter)) continue;
		
		if (IsCustomHitCondition(TargetCharacter) == false) continue;
		
		IgnoreActors.AddUnique(TargetCharacter);
		OnDamageToHitActor(TargetCharacter);
	}
}