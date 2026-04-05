#include "System/TwoMinActorPoolSubsystem.h"
#include "Interfaces/PoolableActorInterface.h"

bool UTwoMinActorPoolSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);
	if (!World) return false;

	return World->WorldType == EWorldType::Game || World->WorldType == EWorldType::PIE;
}

void UTwoMinActorPoolSubsystem::Deinitialize()
{
	PoolMap.Empty();
	Super::Deinitialize();
}

AActor* UTwoMinActorPoolSubsystem::AcquireActor(TSubclassOf<AActor> ActorClass, const FVector& Location,
	const FRotator& Rotation, AActor* Owner)
{
	if (!ActorClass) return nullptr;

	FActorPoolBucket& Bucket = PoolMap.FindOrAdd(ActorClass);

	AActor* Actor = nullptr;

	while (Bucket.InactiveActors.Num() > 0)
	{
		AActor* Candidate = Bucket.InactiveActors.Pop();
		if (IsValid(Candidate))
		{
			Actor = Candidate;
			break;
		}
	}

	if (!Actor)
	{
		Actor = SpawnPooledActor(ActorClass);
	}

	if (!Actor) return nullptr;

	if (IPoolableActorInterface* Poolable = Cast<IPoolableActorInterface>(Actor))
	{
		Poolable->ActivateFromPool(Location, Rotation, Owner);
	}

	return Actor;
}

void UTwoMinActorPoolSubsystem::ReleaseActor(AActor* Actor)
{
	if (!IsValid(Actor)) return;

	IPoolableActorInterface* Poolable = Cast<IPoolableActorInterface>(Actor);
	if (!Poolable) return;

	Poolable->DeactivateToPool();

	FActorPoolBucket& Bucket = PoolMap.FindOrAdd(Actor->GetClass());
	Bucket.InactiveActors.Add(Actor);
}

AActor* UTwoMinActorPoolSubsystem::SpawnPooledActor(TSubclassOf<AActor> ActorClass)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* NewActor = World->SpawnActor(ActorClass, nullptr, nullptr, SpawnParams);
	if (!NewActor) return nullptr;

	if (IPoolableActorInterface* Poolable = Cast<IPoolableActorInterface>(NewActor))
	{
		Poolable->SetOwningPool(this);
		Poolable->DeactivateToPool();
	}

	return NewActor;
}
