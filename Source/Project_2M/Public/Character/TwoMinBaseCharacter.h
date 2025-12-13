
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "Interfaces/BaseUIInterface.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "TwoMinBaseCharacter.generated.h"

class UGameplayEffect;
class UTwoMinAttributeSet;
class UMotionWarpingComponent;
class UBaseCombatComponent;
class UDataAsset_StartUpDataBase;
class UTwoMinAbilitySystemComponent;

UCLASS()
class PROJECT_2M_API ATwoMinBaseCharacter : public ACharacter, public IBaseUIInterface
{
	GENERATED_BODY()

public:
	ATwoMinBaseCharacter();

	virtual UBaseCombatComponent* GetCombatComponent() const;
	virtual UBaseUIComponent* GetBaseUIComponent() const override;
	virtual void BeforeDeathProcess();
	virtual void AfterDeathProcess();
	
	FTimerHandle HitStopTimerHandle;
	
protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UTwoMinAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	UTwoMinAttributeSet* AttributeSetBase;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

	UPROPERTY(VisibleAnywhere, Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float RemoveDelay;
	
	UPROPERTY()
	ECharacterType CharacterType;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect|Health_Gain")
	TSubclassOf<UGameplayEffect> HealthGainEffect;
	
public:
	FORCEINLINE UTwoMinAbilitySystemComponent* GetAbilitySystemComponent() const
	{ return AbilitySystemComponent; }

	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const
	{ return MotionWarpingComponent; }

	FORCEINLINE ECharacterType GetCharacterType() const
	{ return CharacterType; }
	
	FORCEINLINE UGameplayEffect* GetHealthGainEffect() const
	{ return HealthGainEffect->GetDefaultObject<UGameplayEffect>(); }
	
};
