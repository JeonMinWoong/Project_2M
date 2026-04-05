#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableActorInterface.generated.h"

class UTwoMinActorPoolSubsystem;

UINTERFACE(MinimalAPI)
class UPoolableActorInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_2M_API IPoolableActorInterface
{
	GENERATED_BODY()

public:
	virtual void ActivateFromPool(const FVector& Location, const FRotator& Rotation, AActor* NewOwner) = 0;
	virtual void DeactivateToPool() = 0;
	virtual void SetOwningPool(UTwoMinActorPoolSubsystem* InPool) = 0;
	virtual void ReturnToPool() = 0;
};
