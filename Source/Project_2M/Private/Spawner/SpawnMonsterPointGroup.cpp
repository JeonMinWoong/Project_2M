

#include "Spawner/SpawnMonsterPointGroup.h"

#include "TwoMinDebugHelper.h"
#include "GameInstance/TwoMinGameInstance.h"
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
	bool bIsMatchLevel = CurrentName == GI->StateManager->GetCurrentStageName(IndexValue);
	
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
		
		SpawnedMonstersMap.Add(SpawnIndex, SpawnEnemy);
	}
}

