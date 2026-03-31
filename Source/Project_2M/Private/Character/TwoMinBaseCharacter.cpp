
#include "Character/TwoMinBaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/Weapon/TwoMinWeaponBase.h"

ATwoMinBaseCharacter::ATwoMinBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GetMesh()->bReceivesDecals = false;

	AbilitySystemComponent =
		CreateDefaultSubobject<UTwoMinAbilitySystemComponent>(TEXT("ToMinAbilitySystemComponent"));

	AttributeSetBase = CreateDefaultSubobject<UTwoMinAttributeSet>(TEXT("AttributeSetBase"));
	
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UBaseCombatComponent* ATwoMinBaseCharacter::GetCombatComponent() const
{
	return nullptr;
}

UBaseUIComponent* ATwoMinBaseCharacter::GetBaseUIComponent() const
{
	return nullptr;
}

void ATwoMinBaseCharacter::BeforeDeathProcess()
{
	GetCombatComponent()->SetIsAlive(false);
	GetBaseUIComponent()->RemoveFromRoot();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ATwoMinBaseCharacter::AfterDeathProcess()
{
	GetMesh()->bPauseAnims = true;
	
	TArray<ATwoMinWeaponBase*> WeaponBases = GetCombatComponent()->GetCharacterCurrentEquippedWeapon();
	for (ATwoMinWeaponBase* WeaponBase : WeaponBases)
	{
		if (WeaponBase)
		{
			if (RemoveDelay >= 0)
			{
				WeaponBase->SetLifeSpan(RemoveDelay);	
				WeaponBase->StartDissolveProcess();
			}
		}
	}
		
	if (RemoveDelay >= 0)
	{
		if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(this)) return;
		
		SetLifeSpan(RemoveDelay);	
		StartDissolveProcess();
	}
	
}

void ATwoMinBaseCharacter::StartDissolveProcess()
{
	
}

void ATwoMinBaseCharacter::Destroyed()
{
	Super::Destroyed();
	
	OnDestroyedProcess();
}

void ATwoMinBaseCharacter::OnDestroyedProcess()
{
	
}

void ATwoMinBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void ATwoMinBaseCharacter::OnHitEffectSpawnPoint(int32 WeaponIndex)
{
	if (HitEffect_PointSpawnEffect)
	{
		ATwoMinWeaponBase* HitWeapon = GetCombatComponent()->GetCharacterCurrentEquippedWeapon()[WeaponIndex];
		if(!HitWeapon) return;

		FVector SpawnLocation = HitWeapon->GetWeaponMesh()->GetSocketLocation(HitEffect_SpawnSocketName);
		FRotator SpawnRotation = HitWeapon->GetWeaponMesh()->GetSocketRotation(HitEffect_SpawnSocketName);
		
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitEffect_PointSpawnEffect,
			SpawnLocation,
			SpawnRotation
		);
	}
}

void ATwoMinBaseCharacter::OnHitEffectAttachToWeaponSocket(int32 WeaponIndex)
{
	if (HitEffect_AttachTrailEffect)
	{
		if (WeaponIndex > 0) return;
		
		ATwoMinWeaponBase* FirstWeapon = GetCombatComponent()->GetCharacterCurrentEquippedWeapon()[WeaponIndex];
		if(!FirstWeapon) return;		

		UNiagaraFunctionLibrary::SpawnSystemAttached(
			HitEffect_AttachTrailEffect,
			FirstWeapon->GetWeaponMesh(),
			HitEffect_AttachSocketName,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);
	}
}

void ATwoMinBaseCharacter::SetIsAiming(bool bOn)
{
	
}

void ATwoMinBaseCharacter::SetThrowProjectile(ATwoMinProjectileBase* NewThrowProjectile)
{
	
}

void ATwoMinBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);	
	}
}
