// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/Combat/EnemyCombatComponent.h"

#include "TwoMinFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"


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

bool UEnemyCombatComponent::IsAttackCondition(const AActor* TargetActor, int AttackConditionIndex) const
{
	bool bIsSuccessful = false;
	if (AttackConditions.IsEmpty() || !AttackConditions.Contains(AttackConditionIndex)) return bIsSuccessful;
	
	AActor* MyActor = GetOwner();
	if (!MyActor) return bIsSuccessful;
	
	FEnemyAIAttackConditionData AttackCondition = AttackConditions[AttackConditionIndex];
	float DistToBattleTarget = FVector::Dist(MyActor->GetActorLocation(), TargetActor->GetActorLocation());
	
	if (AttackCondition.MinAttackRange > DistToBattleTarget || DistToBattleTarget >= AttackCondition.MaxAttackRange)
	{
		return bIsSuccessful;
	}

	FVector MyForward = MyActor->GetActorForwardVector();
	FVector TargetLocation = (TargetActor->GetActorLocation() - MyActor->GetActorLocation()).GetSafeNormal2D();
	float AngleDeg = UKismetMathLibrary::DegAcos(FVector::DotProduct(MyForward, TargetLocation));

	if (AngleDeg > AttackCondition.TargetAngle)
	{
		return bIsSuccessful;
	}

	if (AttackCondition.AbilityCooldownTag == FGameplayTag::EmptyTag)
	{
		bIsSuccessful = true;
	}
	else
	{
		bIsSuccessful = UTwoMinFunctionLibrary::HasGameplayTag(MyActor, AttackCondition.AbilityCooldownTag) == false;
	}

	if (AttackCondition.ShouldNotExistTag != FGameplayTag::EmptyTag)
	{
		bIsSuccessful = bIsSuccessful ?
		UTwoMinFunctionLibrary::HasGameplayTag(MyActor, AttackCondition.ShouldNotExistTag) == false : false;
	}

	if (AttackCondition.ShouldExistTag == FGameplayTag::EmptyTag)
	{
		return bIsSuccessful;
	}

	bIsSuccessful = bIsSuccessful ? UTwoMinFunctionLibrary::HasGameplayTag(MyActor, AttackCondition.ShouldExistTag) : false;
	return bIsSuccessful;
}

bool UEnemyCombatComponent::IsEvenOneAttackCondition(const AActor* TargetActor) const
{
	AActor* MyActor = GetOwner();
	if (!MyActor) return false;
	
	if (AttackConditions.IsEmpty()) return false;
	
	bool bIsEvenOne = false;
	for (const auto AttackConditionData : AttackConditions)
	{
		if (IsAttackCondition(TargetActor, AttackConditionData.Key))
		{
			bIsEvenOne = true;
		}
	}

	return bIsEvenOne;
}


bool UEnemyCombatComponent::IsEvenOneAttackCooldown()
{
	AActor* MyActor = GetOwner();
	if (!MyActor) return false;
	
	if (AttackConditions.IsEmpty()) return false;
	
	bool bCanAttack = false;
	for (const auto AttackConditionData : AttackConditions)
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(MyActor, AttackConditionData.Value.AbilityCooldownTag) == false)
		{
			bCanAttack = true;
		}
	}

	return bCanAttack;
}

bool UEnemyCombatComponent::IsStopBattleMoveGameplayContainer()
{
	bool bHasStopBattle = false;
	for (auto GameplayTag : StopBattleTags)
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(GetOwner(), GameplayTag))
		{
			bHasStopBattle = true;
		}
	}
	
	return bHasStopBattle;
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
