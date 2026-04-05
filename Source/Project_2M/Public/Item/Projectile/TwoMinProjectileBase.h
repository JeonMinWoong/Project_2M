#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PoolableActorInterface.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinProjectileBase.generated.h"

class USphereComponent;
class ATwoMinBaseCharacter;
class UNiagaraSystem;
class UNiagaraComponent;
class UProjectileMovementComponent;
class UBoxComponent;
class UTwoMinActorPoolSubsystem;

UCLASS()
class PROJECT_2M_API ATwoMinProjectileBase : public AActor, public IPoolableActorInterface
{
	GENERATED_BODY()
	
public:
	ATwoMinProjectileBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	FVector GetSpawnLocation();
	void SetCustomMesh(UStaticMesh* NewMesh) const;
	
	void RecallProjectile();
	
protected:
	void HomingTick(float DeltaSeconds);
	AActor* UpdateHomingTarget();
	
	void FallingTick(float DeltaSeconds);
	
	void UpdateStaticMeshRotation(float DeltaSeconds) const;
	
	void UpdateTargetTick();
	
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
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
	USphereComponent* ProjectileOverlapSphere;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	EProjectileType ProjectileType = EProjectileType::Normal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	EProjectilePierceType ProjectilePierceType = EProjectilePierceType::NonPiercing;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	bool bIsCustomMesh = false;
	
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
	
	TFunction<void()> DestroyCallback;
	
#pragma region FallingData
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|FallingType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Falling"))
	float FallingStartTime = 0;
	
	UPROPERTY()
	float CurFallingTime;
	
	UPROPERTY()
	bool bIsFallingStart = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|FallingType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Falling"))
	float FallingGravityCoef;

	UPROPERTY()
	float CurFallingGravity;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|FallingType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Falling"))
	bool bIsKeepHitFloorProjectile = false;
	
	UPROPERTY()
	bool bIsHitFloor = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|FallingType",
		meta = (EditCondition = "ProjectileType == EProjectileType::Falling"))
	bool bIsOverlapEvent = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|RecallProjectile")
	bool bIsPossibleRecallProjectile = false;
	
	UPROPERTY()
	bool bIsRecallProjectile = false;
	
#pragma endregion
	
	
#pragma region RotationStaticMesh

	UPROPERTY()
	UStaticMeshComponent* CachedStaticMeshComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|StaticMesh")
	bool bIsRotationStaticMesh = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Deails|StaticMesh", 
		meta=(EditCondition = "bIsRotationStaticMesh"))
	FRotator RotationSpeed;
	
#pragma endregion

	
public:
	// IPoolableActorInterface
	virtual void ActivateFromPool(const FVector& Location, const FRotator& Rotation, AActor* NewOwner) override;
	virtual void DeactivateToPool() override;
	virtual void SetOwningPool(UTwoMinActorPoolSubsystem* InPool) override;
	virtual void ReturnToPool() override;

private:
	FVector GetDirection() const;

	void PickUpProjectileProcess(AActor* OwnerActor);
	void DestroyProjectile();
	void ResetProjectileState();

	FAttackInfoData ProjectileAttackInfoData;

	FGameplayTag ActiveAbilityTag;

	UPROPERTY()
	ATwoMinBaseCharacter* CachedTargetCharacter;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> ProjectileAttackGameplayEffectClass;

	int AbilityLevel;

	FTimerHandle PoolReturnTimerHandle;
	bool bIsPooled = false;

	UPROPERTY()
	TObjectPtr<UTwoMinActorPoolSubsystem> OwningPoolSubsystem;

	UPROPERTY()
	TArray<TObjectPtr<UNiagaraComponent>> CachedNiagaraComponents;
	
public:
	FORCEINLINE EProjectileType GetProjectileType() const { return ProjectileType; }
	
	FORCEINLINE void SetProjectileAttackInfoData(const FAttackInfoData& InAttackInfoData)
	{ ProjectileAttackInfoData = InAttackInfoData; }

	FORCEINLINE void SetActiveAbilityTag(const FGameplayTag InTag) { ActiveAbilityTag = InTag; }
	
	FORCEINLINE void SetProjectileAttackGameplayEffectClass(TSubclassOf<UGameplayEffect> InGameplayEffect)
	{ ProjectileAttackGameplayEffectClass = InGameplayEffect; }

	FORCEINLINE void SetActiveAbilityLevel(const int InLevel) { AbilityLevel = InLevel; }
	
	FORCEINLINE void SetTargetCharacter(ATwoMinBaseCharacter* InCharacter) { CachedTargetCharacter = InCharacter; }
	
	FORCEINLINE bool IsCustomMesh() const { return bIsCustomMesh; }
	
	FORCEINLINE void SetOnDestroyedCallback(TFunction<void()> InCallback) { DestroyCallback = InCallback; }
	
	FORCEINLINE bool IsPossibleRecallProjectile() const { return bIsPossibleRecallProjectile && bIsHitFloor; }
};
