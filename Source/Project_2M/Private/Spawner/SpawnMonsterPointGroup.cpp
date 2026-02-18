

#include "Spawner/SpawnMonsterPointGroup.h"

#include "TwoMinDebugHelper.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Item/TwoMinStageWall.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/WorldStageManager.h"
#include "Spawner/SpawnMonsterPoint.h"

ASpawnMonsterPointGroup::ASpawnMonsterPointGroup()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASpawnMonsterPointGroup::BeginPlay()
{
	Super::BeginPlay();
	
	if (SpawnMonsterLevel == ESpawnMonsterLevel::None)
	{
		TwoMinDebugHelper::Print("SpawnMonsterLevel is None", FColor::Red);
		return;
	}
	
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetWorld()->GetGameInstance());
	if (!GI) return;

	const FString CurrentName = GetWorld()->RemovePIEPrefix(GetWorld()->GetMapName());
	const int32 IndexValue = static_cast<int32>(SpawnMonsterLevel) - 1;
	bool bIsMatchLevel = CurrentName == GI->StateManager->GetIndexRealStageName(IndexValue);
	
	if (bIsMatchLevel == false)
	{
		TwoMinDebugHelper::Print("Level is no match.", FColor::Red);
		return;
	}
	
	if (SpawnPoints.IsEmpty() || SpawnPoints.Num() == 0)
	{
		TwoMinDebugHelper::Print("SpawnPoints is empty", FColor::Red);
		return;
	}
	
	for (int Index = 0; Index < SpawnPoints.Num(); Index++)
	{
		if (!SpawnPoints.IsValidIndex(Index)) continue;
		if (SpawnPoints[Index] == nullptr)
		{
			FString Str = FString::Printf(TEXT("SpawnPoints[%d] is nullptr"), Index);
			TwoMinDebugHelper::Print(Str, FColor::Red);
			continue;
		}
		
		int32 SpawnIndex = 0;
		ATwoMinEnemyCharacter* SpawnEnemy = SpawnPoints[Index]->SpawnMonsterPoint(SpawnIndex);
		if (!SpawnEnemy)
		{
			TwoMinDebugHelper::Print("SpawnEnemy is nullptr", FColor::Red);
			return;
		}
		
		if (SpawnEnemy->IsUseBossHealthBar())
		{
			SpawnedBossMonster = SpawnEnemy;
			continue;
		}
		
		SpawnedMonstersMap.Add(SpawnIndex, SpawnEnemy);
	}
	
	MaxDeathMonsterCount = SpawnedMonstersMap.Num();
	
	TArray<AActor*> FoundStageWallActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATwoMinStageWall::StaticClass(), FoundStageWallActors);

	for (AActor* FoundStageWallActor : FoundStageWallActors)
	{
		if (ATwoMinStageWall* StageWall = Cast<ATwoMinStageWall>(FoundStageWallActor))
		{
			StageWallGroup.Emplace(StageWall);
		}
	}
}

void ASpawnMonsterPointGroup::AddDeathMonsterCount()
{
	CurDeathMonsterCount++;
	
	if (CurDeathMonsterCount < MaxDeathMonsterCount) return;
	
	OpenBossStage();
}

void ASpawnMonsterPointGroup::OpenBossStage()
{
	for (auto StageWall : StageWallGroup)
	{
		if (!StageWall) continue;
		
		StageWall->Destroy();
	}
}


