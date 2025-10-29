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
	if (AttackConditions.IsEmpty() || !AttackConditions.Contains(AttackConditionIndex)) return false;
	
	AActor* MyActor = GetOwner();
	if (!MyActor) return false;
	
	FEnemyAIAttackConditionData AttackCondition = AttackConditions[AttackConditionIndex];
	float DistToBattleTarget = FVector::Dist(MyActor->GetActorLocation(), TargetActor->GetActorLocation());
	
	if (AttackCondition.MinAttackRange > DistToBattleTarget || DistToBattleTarget >= AttackCondition.MaxAttackRange)
	{
		return false;
	}

	FVector MyForward = MyActor->GetActorForwardVector();
	FVector TargetLocation = (TargetActor->GetActorLocation() - MyActor->GetActorLocation()).GetSafeNormal2D();
	float AngleDeg = UKismetMathLibrary::DegAcos(FVector::DotProduct(MyForward, TargetLocation));

	if (AngleDeg > AttackCondition.TargetAngle)
	{
		return false;
	}

	if (AttackCondition.AbilityCooldownTag == FGameplayTag::EmptyTag)
	{
		return true;
	}
	
	return UTwoMinFunctionLibrary::HasGameplayTag(MyActor, AttackCondition.AbilityCooldownTag) == false;
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
