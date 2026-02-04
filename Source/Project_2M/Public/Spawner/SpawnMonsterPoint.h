// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "SpawnMonsterPoint.generated.h"

class ATwoMinEnemyCharacter;
struct FEnemyAIPatrolData;
enum class ESpawnMonsterType : uint8;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API ASpawnMonsterPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	
	ATwoMinEnemyCharacter* SpawnMonsterPoint(int32& OutSpawnMonsterPointIndex);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "SpawnInfo|SpawnMonsterPointIndex")
	int32 SpawnMonsterPointIndex = -1;
	
	UPROPERTY(EditAnywhere, Category = "SpawnInfo|IsBoss")
	bool bIsBoss = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "SpawnInfo|SpawnMonsters")
	TMap<ESpawnMonsterType, TSubclassOf<AActor>> SpawnMonsterClasses;
	
	UPROPERTY(EditAnywhere, Category = "SpawnInfo|SpawnMonsterType")
	ESpawnMonsterType SpawnMonsterType;
	
	UPROPERTY(EditAnywhere, Category = "SpawnInfo|Patrol")
	TArray<FEnemyAIPatrolData> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "SpawnInfo|Patrol", meta = (EditCondition = "bCanEditPatrolPathMode"))
	EPatrolPathMode PatrolPathMode = EPatrolPathMode::None;
	
	UPROPERTY()
	bool bCanEditPatrolPathMode = false;
};
