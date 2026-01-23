// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/Combat/EnemyCombatComponent.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Character/TwoMinPlayerCharacter.h"
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

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor, int32 HitWeaponIndex)
{
	Super::OnHitTargetActor(HitActor, HitWeaponIndex);
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

bool UEnemyCombatComponent::IsEvasionCondition(AActor* TargetActor, EEnemyEvasionType EvasionType) const
{
	bool bIsSuccessful = false;
	if (EvasionConditions.IsEmpty() || !EvasionConditions.Contains(EvasionType)) return bIsSuccessful;
	
	AActor* MyActor = GetOwner();
	if (!MyActor) return bIsSuccessful;
	
	if (UTwoMinFunctionLibrary::HasGameplayTag(TargetActor, TwoMinGameplayTag::Player_State_BeforeAttacking) == false)
	{
		return bIsSuccessful;
	}
	
	FEnemyAIEvasionConditionDate EvasionCondition = EvasionConditions[EvasionType];
	float DistToBattleTarget = FVector::Dist(MyActor->GetActorLocation(), TargetActor->GetActorLocation());
	
	if (EvasionCondition.PossibleMinDistance >= DistToBattleTarget || 
		EvasionCondition.PossibleMaxDistance < DistToBattleTarget)
	{
		return bIsSuccessful;
	}

	FVector MyForward = MyActor->GetActorForwardVector();
	FVector ToTarget = (TargetActor->GetActorLocation() - MyActor->GetActorLocation()).GetSafeNormal2D();
	float Dot = FVector::DotProduct(MyForward, ToTarget);
	float Cross = FVector::CrossProduct(MyForward, ToTarget).Z;
	float AngleDeg = FMath::RadiansToDegrees(FMath::Atan2(Cross, Dot));

	if (EvasionCondition.PossibleMinAngle > AngleDeg || EvasionCondition.PossibleMaxAngle < AngleDeg)
	{
		return bIsSuccessful;
	}

	if (EvasionCondition.AbilityCooldownTag == FGameplayTag::EmptyTag)
	{
		bIsSuccessful = true;
	}
	else
	{
		bIsSuccessful = UTwoMinFunctionLibrary::HasGameplayTag(MyActor, EvasionCondition.AbilityCooldownTag) == false;
	}

	if (EvasionCondition.ShouldNotExistTag != FGameplayTag::EmptyTag)
	{
		bIsSuccessful = bIsSuccessful ?
		UTwoMinFunctionLibrary::HasGameplayTag(MyActor, EvasionCondition.ShouldNotExistTag) == false : false;
	}

	if (EvasionCondition.ShouldExistTag == FGameplayTag::EmptyTag)
	{
		return bIsSuccessful;
	}

	bIsSuccessful = bIsSuccessful ? UTwoMinFunctionLibrary::HasGameplayTag(MyActor, EvasionCondition.ShouldExistTag) : false;
	return bIsSuccessful;
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

void UEnemyCombatComponent::UpdateBanAttack(float MinBanAttackDelay, float MaxBanAttackDelay)
{
	AActor* EnemyCharacter = GetOwner();
	
	if (bIsBanAttack)
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(EnemyCharacter, TwoMinGameplayTag::Enemy_State_BanAttack)) return;
	}
	else
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(EnemyCharacter, TwoMinGameplayTag::Enemy_State_NoBanAttack)) return;
	}
	
	bool NextBanAttack = UKismetMathLibrary::RandomBool();
	if (NextBanAttack)
	{
		if (bIsBanAttack == NextBanAttack)
		{
			NextBanAttack = !NextBanAttack;	
		}
	}

	ClearBanAttack();
	bIsBanAttack = NextBanAttack;
	
	TwoMinDebugHelper::Print(TEXT("공격 상태 : ") + FString(bIsBanAttack ? TEXT("금지") : TEXT("가능")));
	
	if (bIsBanAttack)
	{
		float BanAttackDelay = FMath::FRandRange(MinBanAttackDelay, MaxBanAttackDelay);
		UTwoMinFunctionLibrary::AddGameplayTagToActor(EnemyCharacter, TwoMinGameplayTag::Enemy_State_BanAttack);
		FTimerManager& TimerManager = EnemyCharacter->GetWorldTimerManager();
		TimerManager.SetTimer(this->BanAttackTimerHandle,
			[EnemyCharacter]()
			{
				UTwoMinFunctionLibrary::RemoveGameplayTagToActor(EnemyCharacter, TwoMinGameplayTag::Enemy_State_BanAttack);
			}, BanAttackDelay, false);
	}
	else
	{
		float NoBanAttackDelay = 8.f;
		UTwoMinFunctionLibrary::AddGameplayTagToActor(EnemyCharacter, TwoMinGameplayTag::Enemy_State_NoBanAttack);
		FTimerManager& TimerManager = EnemyCharacter->GetWorldTimerManager();
		TimerManager.SetTimer(this->BanAttackTimerHandle,
			[EnemyCharacter]()
			{
				UTwoMinFunctionLibrary::RemoveGameplayTagToActor(EnemyCharacter, TwoMinGameplayTag::Enemy_State_NoBanAttack);
			}, NoBanAttackDelay, false);
	}
}

void UEnemyCombatComponent::ClearBanAttack()
{
	bIsBanAttack = false;
	
	AActor* EnemyCharacter = GetOwner();
	FTimerManager& TimerManager = EnemyCharacter->GetWorldTimerManager();
	TimerManager.ClearTimer(this->BanAttackTimerHandle);
	UTwoMinFunctionLibrary::RemoveGameplayTagToActor(EnemyCharacter, TwoMinGameplayTag::Enemy_State_BanAttack);
	UTwoMinFunctionLibrary::RemoveGameplayTagToActor(EnemyCharacter, TwoMinGameplayTag::Enemy_State_NoBanAttack);
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
