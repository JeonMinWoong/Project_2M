
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "SpawnMonsterPointGroup.generated.h"

class ATwoMinEnemyCharacter;
class ASpawnMonsterPoint;

UCLASS()
class PROJECT_2M_API ASpawnMonsterPointGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnMonsterPointGroup();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "SpawnGroupInfo|SpawnLevel")
	ESpawnMonsterLevel SpawnMonsterLevel;
	
	UPROPERTY(EditAnywhere, Category = "SpawnGroupInfo|SpawnPoints")
	TArray<ASpawnMonsterPoint*> SpawnPoints;
	
	UPROPERTY(VisibleAnywhere, Category = "SpawnGroupInfo|SpawnedMonsters")
	TMap<int32, ATwoMinEnemyCharacter*> SpawnedMonstersMap;
	
};
