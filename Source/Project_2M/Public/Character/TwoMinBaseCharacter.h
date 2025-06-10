
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TwoMinBaseCharacter.generated.h"

class UDataAsset_StartUpDataBase;
class UToMinAbilitySystemComponent;

UCLASS()
class PROJECT_2M_API ATwoMinBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATwoMinBaseCharacter();

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UToMinAbilitySystemComponent* ToMinAbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;
	
};
