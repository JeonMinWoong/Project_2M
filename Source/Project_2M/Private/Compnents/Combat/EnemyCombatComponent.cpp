// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/Combat/EnemyCombatComponent.h"


UEnemyCombatComponent::UEnemyCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsPatrol = false;
}

void UEnemyCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PatrolWaitPoint();
}

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Super::OnHitTargetActor(HitActor);
}

int32 UEnemyCombatComponent::GetNextPatrolPointIndex()
{
	int32 NextIndex = 0;
	
	switch (PatrolPathMode)
	{
	case EPatrolPathMode::Loop:
		CurrentPatrolPointIndex++;
		CurrentPatrolPointIndex %= PatrolPoints.Num();
		NextIndex = FMath::Clamp(CurrentPatrolPointIndex, 0, PatrolPoints.Num() - 1);
		break;
	case EPatrolPathMode::PingPong:
		if (CurrentPatrolPointIndex >= PatrolPoints.Num() - 1)
		{
			bPingPongForward = false;
		}
		else if (CurrentPatrolPointIndex <= 0)
		{
			bPingPongForward = true;
		}
		
		CurrentPatrolPointIndex = bPingPongForward ? CurrentPatrolPointIndex + 1 : CurrentPatrolPointIndex - 1;
		NextIndex = FMath::Clamp(CurrentPatrolPointIndex, 0, PatrolPoints.Num() - 1);
		break;
	}

	return NextIndex;
}

void UEnemyCombatComponent::ResetPatrolPoint()
{
	bIsPatrolPointArrivedWait = true;
	if (PatrolPoints.IsEmpty())
	{
		bIsPatrol = false;
	}
}

void UEnemyCombatComponent::PatrolWaitPoint()
{
	if (PatrolPoints.IsEmpty()) return;
	if (bIsPatrolPointArrivedWait == false) return;
	
	CurrentPatrolPointArrivedWaitTime += GetWorld()->GetDeltaSeconds();
	if (CurrentPatrolPointArrivedWaitTime < PatrolPoints[CurrentPatrolPointIndex].WaitTime)
	{
		return;
	}

	CurrentPatrolPointArrivedWaitTime = 0.f;
	bIsPatrolPointArrivedWait = false;
	bIsPatrol = false;
}
