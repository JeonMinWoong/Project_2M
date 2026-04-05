#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TwoMinActorPoolSubsystem.generated.h"

class IPoolableActorInterface;

USTRUCT()
struct FActorPoolBucket
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<AActor>> InactiveActors;
};

UCLASS()
class PROJECT_2M_API UTwoMinActorPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Deinitialize() override;

	AActor* AcquireActor(TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation, AActor* Owner);
	void ReleaseActor(AActor* Actor);

	template<typename T>
	T* AcquireActor(TSubclassOf<T> ActorClass, const FVector& Location, const FRotator& Rotation, AActor* Owner)
	{
		return Cast<T>(AcquireActor(TSubclassOf<AActor>(ActorClass), Location, Rotation, Owner));
	}

private:
	AActor* SpawnPooledActor(TSubclassOf<AActor> ActorClass);

	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActorPoolBucket> PoolMap;
};
