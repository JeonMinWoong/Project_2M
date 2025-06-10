
#include "Character/TwoMinBaseCharacter.h"

#include "AbilitySystem/ToMinAbilitySystemComponent.h"

ATwoMinBaseCharacter::ATwoMinBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	ToMinAbilitySystemComponent =
		CreateDefaultSubobject<UToMinAbilitySystemComponent>(TEXT("ToMinAbilitySystemComponent"));
}

void ATwoMinBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ToMinAbilitySystemComponent)
	{
		ToMinAbilitySystemComponent->InitAbilityActorInfo(this, this);	
	}
}
