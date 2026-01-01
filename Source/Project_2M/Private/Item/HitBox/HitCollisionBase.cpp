
#include "Item/HitBox/HitCollisionBase.h"

#include "Character/TwoMinBaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

AHitCollisionBase::AHitCollisionBase()
{
	PrimaryActorTick.bCanEverTick = true;
	HitCollisionType = EHitCollisionType::None;
}

void AHitCollisionBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

void AHitCollisionBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHitCollisionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentHitTerm >= MaxTimer)
	{
		Destroy();
		return;	
	}
	
	if (CurrentHitCount >= MaxHitCount)
	{
		Destroy();
		return;
	}
	
	CurrentHitTerm += DeltaTime;
	if (CurrentHitTerm >= MaxHitTerm)
	{
		CurrentHitCount++;
		OnCheckHitActorCollision();
	}
}

void AHitCollisionBase::Destroyed()
{
	Super::Destroyed();
	
	IgnoreActors.Empty();
}

void AHitCollisionBase::OnCheckHitActorCollision()
{
	
}

void AHitCollisionBase::OnDamageToHitActor(AActor* HitActor)
{
	
}