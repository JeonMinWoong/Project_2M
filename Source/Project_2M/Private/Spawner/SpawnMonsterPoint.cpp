// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/SpawnMonsterPoint.h"

#include "TwoMinDebugHelper.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Spawner/SpawnMonsterPointGroup.h"

#if WITH_EDITOR

void ASpawnMonsterPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (SpawnMonsterPointIndex == -1) 
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), FoundActors);
        
		SpawnMonsterPointIndex = FoundActors.Num();
	}
}

void ASpawnMonsterPoint::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	
	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	const FName ArrayName = GET_MEMBER_NAME_CHECKED(ASpawnMonsterPoint, PatrolPoints);
	
	if (PropertyName != ArrayName) return;
	
	bCanEditPatrolPathMode = PatrolPoints.Num() > 1;
	
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	for (AActor* AttachedActor : AttachedActors)
	{
		ATargetPoint* Point = Cast<ATargetPoint>(AttachedActor);
		if (!Point) continue;
			
		bool bIsFound = false;
		for (int32 Index = 0; Index < PatrolPoints.Num(); ++Index)
		{
			if (PatrolPoints[Index].PatrolPoint == Point)
			{
				bIsFound = true;
				break;
			}
		}
			
		if (bIsFound == false)
		{
			Point->Destroy();
		}
	}
		
	for (int32 Index = 0; Index < PatrolPoints.Num(); ++Index)
	{
		if (PatrolPoints.IsValidIndex(Index) && PatrolPoints[Index].PatrolPoint == nullptr)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			ATargetPoint* NewPoint = GetWorld()->SpawnActor<ATargetPoint>(
				ATargetPoint::StaticClass(), 
				GetActorLocation(), 
				GetActorRotation(), 
				SpawnParams
			);

			if (!NewPoint) continue;

			NewPoint->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			PatrolPoints[Index].PatrolPoint = NewPoint;
		}
	}
	
}

#endif

ATwoMinEnemyCharacter* ASpawnMonsterPoint::SpawnMonsterPoint(int32& OutSpawnMonsterPointIndex)
{
	OutSpawnMonsterPointIndex = SpawnMonsterPointIndex;
	if (SpawnMonsterType == ESpawnMonsterType::None)
	{
		FString Str = FString::Printf(TEXT("SpawnMonsterType[SpawnMonsterPointIndex(%d)] is None"), SpawnMonsterPointIndex);
		TwoMinDebugHelper::Print(Str, FColor::Red);
		return nullptr;
	}
	
	if (SpawnMonsterClasses.Contains(SpawnMonsterType) == false)
	{
		FString Str = FString::Printf(TEXT("SpawnMonsterClass[SpawnMonsterPointIndex(%d)] is null"), SpawnMonsterPointIndex);
		TwoMinDebugHelper::Print(Str, FColor::Red);
		return nullptr;
	}
	
	ATwoMinEnemyCharacter* SpawnEnemy = 
		GetWorld()->SpawnActorDeferred<ATwoMinEnemyCharacter>(
			SpawnMonsterClasses[SpawnMonsterType], 
			GetActorTransform(), 
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);
	
	if (!SpawnEnemy)
	{
		FString Str = FString::Printf(TEXT("SpawnEnemy[SpawnMonsterPointIndex(%d)] is null"), SpawnMonsterPointIndex);
		TwoMinDebugHelper::Print(Str, FColor::Red);
		return nullptr;
	}
	
	UEnemyCombatComponent* EnemyCombatComponent = Cast<UEnemyCombatComponent>(SpawnEnemy->GetCombatComponent());
	if (!EnemyCombatComponent) return nullptr;
	
	if (PatrolPoints.Num() > 1 && PatrolPathMode != EPatrolPathMode::None)
	{
		EnemyCombatComponent->InitPatrol(PatrolPoints, PatrolPathMode);	
	}
	
	SpawnEnemy->SetUseBossHealthBar(bIsBoss);
	SpawnEnemy->FinishSpawning(GetActorTransform());
	return SpawnEnemy;
}
