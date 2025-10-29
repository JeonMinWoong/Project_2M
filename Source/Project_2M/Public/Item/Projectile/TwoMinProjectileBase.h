#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinProjectileBase.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class PROJECT_2M_API ATwoMinProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	ATwoMinProjectileBase();

protected:
	
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);
	
	void HandleApplyProjectileDamage(APawn* InHitPawn, FGameplayEventData& InPayLoad, bool bIsTargetGuard) const;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UBoxComponent* ProjectileCollisionBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	EProjectileType ProjectileType = EProjectileType::Normal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	EProjectilePierceType ProjectilePierceType = EProjectilePierceType::NonPiercing;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails")
	float InitialSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails")
	float MaxSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails")
	float LifeTime = 2.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Deails|HomingType",
		meta = (EditCondition = "bIsHoming"))
	AActor* HomingTarget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|HomingType",
			meta = (EditCondition = "bIsHoming"))
	float HomingActivationDelay = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|HomingType",
			meta = (EditCondition = "bIsHoming"))
	float HomingRetargetInterval = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|HomingType",
			meta = (EditCondition = "bIsHoming"))
	float HomingChaseTime = 5.f;
	
	bool bIsHoming = false;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		bIsHoming = (ProjectileType == EProjectileType::Homing);
	}
#endif
	
private:
	FAttackInfoData ProjectileAttackInfoData;
	
	FGameplayTag ActiveAbilityTag;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> ProjectileAttackGameplayEffectClass;

	int AbilityLevel;
	
public:
	FORCEINLINE void SetProjectileAttackInfoData(const FAttackInfoData& InAttackInfoData)
	{ ProjectileAttackInfoData = InAttackInfoData; }

	FORCEINLINE void SetActiveAbilityTag(const FGameplayTag InTag) { ActiveAbilityTag = InTag; }
	
	FORCEINLINE void SetProjectileAttackGameplayEffectClass(TSubclassOf<UGameplayEffect> InGameplayEffect)
	{ ProjectileAttackGameplayEffectClass = InGameplayEffect; }

	FORCEINLINE void SetActiveAbilityLevel(const int InLevel) { AbilityLevel = InLevel; }
};
