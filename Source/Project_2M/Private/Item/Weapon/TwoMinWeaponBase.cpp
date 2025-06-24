// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/TwoMinWeaponBase.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Item/Weapon/TwoMinWeaponPlayer.h"
#include "ToMinTypes/TwoMinStructTypes.h"

ATwoMinWeaponBase::ATwoMinWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMesh);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
	
}

FTwoMinPlayerWeaponData ATwoMinWeaponBase::GetWeaponData() const
{
	return FTwoMinPlayerWeaponData();
}

void ATwoMinWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	if (!WeaponOwningPawn) return;
	
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UTwoMinFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			FString HitPlayerName = FString::Printf(TEXT("Hit Pawn! : %s"), *HitPawn->GetActorNameOrLabel());
			DebugTwoMin::Print(HitPlayerName, FColor::Green);

			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}

void ATwoMinWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	if (!WeaponOwningPawn) return;
	
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UTwoMinFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
		}
	}
}

