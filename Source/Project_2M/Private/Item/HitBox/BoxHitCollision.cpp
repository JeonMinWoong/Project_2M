// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HitBox/BoxHitCollision.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

ABoxHitCollision::ABoxHitCollision()
{
	HitCollisionType = EHitCollisionType::Box;
}

void ABoxHitCollision::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (bDrawEditorDebug)
	{
#if WITH_EDITOR
		
		FQuat Rotation = GetActorQuat();
		FVector Center = GetActorLocation() + Rotation.RotateVector(Offset);

		DrawDebugBox(
			GetWorld(),
			Center,
			BoxExtent,
			Rotation,
			FColor::Green,
			false,
			3.f,
			0,
			2.f
		);
#endif	
	}
}

void ABoxHitCollision::OnCheckHitActorCollision()
{
	Super::OnCheckHitActorCollision();
	
	TArray<FHitResult> HitResult;
	FRotator Rotation = GetActorRotation();
	FVector Center = GetActorLocation() + Rotation.RotateVector(Offset);
	
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetWorld(),
		Center,
		Center,
		BoxExtent,
		Rotation,
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
		
		IgnoreActors.AddUnique(TargetCharacter);
		OnDamageToHitActor(TargetCharacter);
	}
}
