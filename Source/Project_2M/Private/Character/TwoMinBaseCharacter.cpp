
#include "Character/TwoMinBaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"

ATwoMinBaseCharacter::ATwoMinBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

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

void ATwoMinBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);	
	}
}
