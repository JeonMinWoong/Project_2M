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
	WeaponMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SetRootComponent(WeaponMesh);
	
	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
	WeaponCollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
}

FTwoMinPlayerWeaponData ATwoMinWeaponBase::GetWeaponData() const
{
	return FTwoMinPlayerWeaponData();
}

void ATwoMinWeaponBase::StartDissolveProcess()
{
	for (int32 Index = 0; Index < WeaponMesh->GetNumMaterials(); Index++)
	{
		UMaterialInterface* Mat = WeaponMesh->GetMaterial(Index);
		UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Mat, this);
    
		WeaponMesh->SetCastShadow(false);
		WeaponMesh->SetMaterial(Index, DynMat);
		CachedDynamicMaterials.Add(DynMat);
	}
	
	GetWorldTimerManager().SetTimer(DissolveTimerHandle, this, &ATwoMinWeaponBase::UpdateDissolveMaterial,
		UpdateDissolveTime, true);
}

void ATwoMinWeaponBase::UpdateDissolveMaterial()
{
	CurDissolve += UpdateDissolveValue;
	for (auto DynamicMaterialInstance : CachedDynamicMaterials)
	{
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("DissolveAmount"), CurDissolve);
	}

	if (CurDissolve >= 1)
	{
		GetWorldTimerManager().ClearTimer(DissolveTimerHandle);
	}
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
			OnWeaponHitTarget.ExecuteIfBound(OtherActor, WeaponIndex);
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
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor, WeaponIndex);
		}
	}
}

