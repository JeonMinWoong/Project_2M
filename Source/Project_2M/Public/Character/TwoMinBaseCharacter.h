
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "Interfaces/BaseUIInterface.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "TwoMinBaseCharacter.generated.h"

class ATwoMinProjectileBase;
class UNiagaraSystem;
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
	ATwoMinBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual UBaseCombatComponent* GetCombatComponent() const;
	virtual UBaseUIComponent* GetBaseUIComponent() const override;
	virtual void BeforeDeathProcess();
	virtual void AfterDeathProcess();
	virtual void StartDissolveProcess();
	virtual void Destroyed() override;
	virtual void OnDestroyedProcess();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void OnHitEffectSpawnPoint(int32 WeaponIndex);
	virtual void OnHitEffectAttachToWeaponSocket(int32 WeaponIndex);
	
	virtual void SetIsAiming(bool bOn);
	virtual void SetThrowProjectile(ATwoMinProjectileBase* NewThrowProjectile);

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
	
	UPROPERTY(EditDefaultsOnly, Category = "MeleeHitEffects|PointSpawnEffect")
	UNiagaraSystem* HitEffect_PointSpawnEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "MeleeHitEffects|PointSpawnEffect")
	FName HitEffect_SpawnSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = "MeleeHitEffects|AttachTrailEffect")
	UNiagaraSystem* HitEffect_AttachTrailEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "MeleeHitEffects|AttachTrailEffect")
	FName HitEffect_AttachSocketName;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect|Health_Gain")
	TSubclassOf<UGameplayEffect> HealthGainEffect;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect|Stamina_Gain")
	TSubclassOf<UGameplayEffect> StaminaGainEffect;
	
	UPROPERTY()
	int32 GainGold;
	
public:
	FORCEINLINE UTwoMinAbilitySystemComponent* GetAbilitySystemComponent() const
	{ return AbilitySystemComponent; }

	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const
	{ return MotionWarpingComponent; }

	FORCEINLINE ECharacterType GetCharacterType() const
	{ return CharacterType; }
	
	FORCEINLINE UGameplayEffect* GetHealthGainEffect() const
	{ return HealthGainEffect->GetDefaultObject<UGameplayEffect>(); }
	
	FORCEINLINE UGameplayEffect* GetStaminaGainEffect() const
	{ return StaminaGainEffect->GetDefaultObject<UGameplayEffect>(); }
	
	FORCEINLINE void SetGainGold(int32 InGainGold) { GainGold = InGainGold; }
	FORCEINLINE int32 GetGainGold() const { return GainGold; }
};
