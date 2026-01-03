// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HitBox/SectorHitCollision.h"

ASectorHitCollision::ASectorHitCollision()
{
	HitCollisionType = EHitCollisionType::Sector;
}

void ASectorHitCollision::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (bDrawEditorDebug)
	{
#if WITH_EDITOR
		
		UWorld* World = GetWorld();
		FVector Forward2D = GetActorQuat().GetForwardVector();

		FQuat Rotation = GetActorQuat();
		FVector Center = GetActorLocation() + Rotation.RotateVector(Offset);

		const float Duration = 3.f;
		const int32 Segments = 16;
		
		float HalfAngle = SectorAngle / 2.f;
		float StartAngle = -HalfAngle;
		float EndAngle = HalfAngle;
		float Step = (EndAngle - StartAngle) / Segments;

		FVector PrevPoint = Center;

		for (int32 i = 0; i <= Segments; ++i)
		{
			float Angle = StartAngle + Step * i;

			FVector Dir = Forward2D.RotateAngleAxis(Angle, FVector::UpVector);
			FVector Point = Center + Dir * Radius;
			
			DrawDebugLine(World, Center, Point, FColor::Red, false, 
				Duration, 0, 2.f);
			
			if (i > 0)
			{
				DrawDebugLine(World, PrevPoint, Point, FColor::Yellow, false, 
					Duration, 0, 2.f);
			}

			PrevPoint = Point;
		}
#endif
	}
}

bool ASectorHitCollision::IsCustomHitCondition(AActor* HitActor)
{
	const FVector DirectionToTarget = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const float DotProduct = FVector::DotProduct(GetActorForwardVector(), DirectionToTarget);
	const float AngleToTarget = FMath::Acos(DotProduct) * (180.f / PI);
	const float HalfAngle = SectorAngle / 2.f;
	
	return AngleToTarget <= HalfAngle;
}
