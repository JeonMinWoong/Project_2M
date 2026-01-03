
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "GameplayEffect.h"
#include "HitCollisionBase.generated.h"

struct FAttackInfoData;

namespace EDrawDebugTrace
{
	enum Type : int;
}

class ATwoMinBaseCharacter;

UCLASS()
class PROJECT_2M_API AHitCollisionBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AHitCollisionBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	
	virtual void OnCheckHitActorCollision();
	virtual void OnDamageToHitActor(AActor* HitActor);
	
	virtual bool IsCustomHitCondition(AActor* HitActor);
	
	UPROPERTY(EditAnywhere, Category="Debug")
	bool bDrawEditorDebug = true;
	
	UPROPERTY()
	EHitCollisionType HitCollisionType;
	
	UPROPERTY()
	ATwoMinBaseCharacter* OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category= "HitObjectType")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	UPROPERTY()
	TArray<AActor*> IgnoreActors;
	
	UPROPERTY(EditDefaultsOnly, Category = "Offset")
	FVector Offset = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditDefaultsOnly, Category = "HitCount")
	int32 MaxHitCount = 1;
	
	UPROPERTY(VisibleAnywhere, Category = "HitCount")
	int32 CurrentHitCount = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "HitTerm")
	float MaxHitTerm = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "HitTerm")
	float CurrentHitTerm = 0.f;
	
	UPROPERTY(VisibleAnywhere)
	float MaxTimer = 10.f;
	
	FAttackInfoData AttackInfoData;
	
	FGameplayTag ActiveAbilityTag;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> CollisionAttackGameplayEffectClass;
	
	int AbilityLevel;
	
public:
	FORCEINLINE EHitCollisionType GetHitCollisionType() const { return HitCollisionType; }
	
	FORCEINLINE void SetCollisionAttackInfoData(const FAttackInfoData& InAttackInfoData)
	{ AttackInfoData = InAttackInfoData; }

	FORCEINLINE void SetActiveAbilityTag(const FGameplayTag InTag) { ActiveAbilityTag = InTag; }
	
	FORCEINLINE void SetCollisionAttackGameplayEffectClass(TSubclassOf<UGameplayEffect> InGameplayEffect)
	{ CollisionAttackGameplayEffectClass = InGameplayEffect; }
	
	FORCEINLINE void SetActiveAbilityLevel(const int InLevel) { AbilityLevel = InLevel; }
};
