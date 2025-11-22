
#include "Character/TwoMinBaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/Weapon/TwoMinWeaponBase.h"

ATwoMinBaseCharacter::ATwoMinBaseCharacter()
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

void ATwoMinBaseCharacter::DeathProcess()
{
	GetMesh()->bPauseAnims = true;
		
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetBaseUIComponent()->RemoveFromRoot();

	TArray<ATwoMinWeaponBase*> WeaponBases = GetCombatComponent()->GetCharacterCurrentEquippedWeapon();
	for (ATwoMinWeaponBase* WeaponBase : WeaponBases)
	{
		if (WeaponBase)
		{
			if (RemoveDelay >= 0)
			{
				WeaponBase->SetLifeSpan(RemoveDelay);	
			}
		}
	}
		
	if (RemoveDelay >= 0)
	{
		SetLifeSpan(RemoveDelay);	
	}

	GetCombatComponent()->SetIsAlive(false);
}

void ATwoMinBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);	
	}
}
