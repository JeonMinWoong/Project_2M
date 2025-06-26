
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TwoMinBaseCharacter.generated.h"

class UMotionWarpingComponent;
class UBaseCombatComponent;
class UDataAsset_StartUpDataBase;
class UTwoMinAbilitySystemComponent;

UCLASS()
class PROJECT_2M_API ATwoMinBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATwoMinBaseCharacter();

	virtual UBaseCombatComponent* GetCombatComponent() const;
	
protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UTwoMinAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

	UPROPERTY(VisibleAnywhere, Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;


public:
	FORCEINLINE UTwoMinAbilitySystemComponent* GetAbilitySystemComponent() const
	{ return AbilitySystemComponent; }

	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const
	{ return MotionWarpingComponent; }
};
