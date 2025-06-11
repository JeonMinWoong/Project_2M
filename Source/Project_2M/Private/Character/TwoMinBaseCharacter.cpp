
#include "Character/TwoMinBaseCharacter.h"

#include "AbilitySystem/TwoMinAbilitySystemComponent.h"

ATwoMinBaseCharacter::ATwoMinBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	TwoMinAbilitySystemComponent =
		CreateDefaultSubobject<UTwoMinAbilitySystemComponent>(TEXT("ToMinAbilitySystemComponent"));
}

UBaseCombatComponent* ATwoMinBaseCharacter::GetCombatComponent() const
{
	return nullptr;
}

void ATwoMinBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (TwoMinAbilitySystemComponent)
	{
		TwoMinAbilitySystemComponent->InitAbilityActorInfo(this, this);	
	}
}
