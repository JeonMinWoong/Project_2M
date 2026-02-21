
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "SpawnMonsterPointGroup.generated.h"

class ATwoMinStageWall;
class ATwoMinEnemyCharacter;
class ASpawnMonsterPoint;

UCLASS()
class PROJECT_2M_API ASpawnMonsterPointGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnMonsterPointGroup();

	void AddDeathMonsterCount();
	
	void OpenBossStage();
	void EnterBossStage();
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "SpawnGroupInfo|SpawnLevel")
	ESpawnMonsterLevel SpawnMonsterLevel;
	
	UPROPERTY(EditAnywhere, Category = "SpawnGroupInfo|SpawnPoints")
	TArray<ASpawnMonsterPoint*> SpawnPoints;
	
	UPROPERTY(VisibleAnywhere, Category = "SpasnGroupInfo|SpawnedMonsters")
	TMap<int32, ATwoMinEnemyCharacter*> SpawnedMonstersMap;
	
	UPROPERTY(VisibleAnywhere, Category = "SpawnGroupInfo|SpawnedBossMonster")
	ATwoMinEnemyCharacter* SpawnedBossMonster;
	
	UPROPERTY(VisibleAnywhere, Category = "SpawnGroupInfo|DeathMonsterCount")
	int32 MaxDeathMonsterCount;
	
	UPROPERTY(VisibleAnywhere, Category = "SpawnGroupInfo|DeathMonsterCount")
	int32 CurDeathMonsterCount = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "SpawnGroupInfo|StageWallGroup")
	TArray<ATwoMinStageWall*> StageWallGroup;
	
};
