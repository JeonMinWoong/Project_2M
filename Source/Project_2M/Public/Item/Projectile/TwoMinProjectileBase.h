#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinProjectileBase.generated.h"

class ATwoMinBaseCharacter;
class UNiagaraSystem;
class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class PROJECT_2M_API ATwoMinProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	ATwoMinProjectileBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	FVector GetSpawnLocation();
	
protected:
	void HomingTick(float DeltaSeconds);
	AActor* UpdateHomingTarget();
	
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);
	
	void HandleApplyProjectileDamage(APawn* InHitPawn, FGameplayEventData& InPayLoad, bool bIsTargetGuard) const;
	void OnHitPlayEffect(const FHitResult& HitResult);
	void PlayImpactEffect(const FHitResult& HitResult) const;
	virtual void Destroyed() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UBoxComponent* ProjectileCollisionBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	EProjectileType ProjectileType = EProjectileType::Normal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	EProjectilePierceType ProjectilePierceType = EProjectilePierceType::NonPiercing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Effect")
	UNiagaraSystem* ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Sound")
	USoundBase* ImpactSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails")
	float InitialSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails")
	float MaxSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails")
	float LifeTime = 2.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails")
	float HoverTime = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Deails|HomingType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Homing"))
	AActor* HomingTarget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|HomingType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Homing"))
	float HomingRange = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|HomingType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Homing"))
	float HomingActivationDelay = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|HomingType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Homing"))
	float HomingRetargetInterval = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|HomingType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Homing"))
	float HomingAccelerationMagnitude = 450.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|LocationType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Location"))
	FVector SpawnLocation = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|LocationType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Location"))
	FVector RandomLocation = FVector::ZeroVector;
	
	bool bIsHit = false;
	float CurHoverTime = 0.f;
	bool bIsHoverOut = false;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;
	
	bool bIsHomingStart = false;
	float CurHomingActivationDelay = 0.f;
	float CurHomingRetargetInterval = 0.f;
	
private:
	FVector GetDirection() const;
	
	FAttackInfoData ProjectileAttackInfoData;
	
	FGameplayTag ActiveAbilityTag;
	
	UPROPERTY()
	ATwoMinBaseCharacter* CachedTargetCharacter;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> ProjectileAttackGameplayEffectClass;

	int AbilityLevel;
	
public:
	FORCEINLINE EProjectileType GetProjectileType() const { return ProjectileType; }
	
	FORCEINLINE void SetProjectileAttackInfoData(const FAttackInfoData& InAttackInfoData)
	{ ProjectileAttackInfoData = InAttackInfoData; }

	FORCEINLINE void SetActiveAbilityTag(const FGameplayTag InTag) { ActiveAbilityTag = InTag; }
	
	FORCEINLINE void SetProjectileAttackGameplayEffectClass(TSubclassOf<UGameplayEffect> InGameplayEffect)
	{ ProjectileAttackGameplayEffectClass = InGameplayEffect; }

	FORCEINLINE void SetActiveAbilityLevel(const int InLevel) { AbilityLevel = InLevel; }
	
	FORCEINLINE void SetTargetCharacter(ATwoMinBaseCharacter* InCharacter) { CachedTargetCharacter = InCharacter; }
};
