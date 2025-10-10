// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_BattleMoveCondition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinBaseCharacter.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTDE_BattleMoveCondition::UBTDE_BattleMoveCondition()
{
	NodeName = "BattleMoveCondition";
}

bool UBTDE_BattleMoveCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = GetBlackboardComponent(OwnerComp);
	if (!BB) return false;

	UObject* Object = BB->GetValueAsObject(TwoMinBBKeys::BattleTarget);
	if (!Object) return false;

	ATwoMinBaseCharacter* BattleTarget = Cast<ATwoMinBaseCharacter>(Object);
	if (!BattleTarget) return false;

	UEnemyCombatComponent* EnemyCombatComponent = GetEnemyCombatComponent(OwnerComp);
	if (!EnemyCombatComponent) return false;
	
	return CheckAttackRange(EnemyCombatComponent->GetOwner(), BattleTarget) && 
		CheckTargetAngle(EnemyCombatComponent->GetOwner(), BattleTarget);
}

bool UBTDE_BattleMoveCondition::CheckAttackRange(const AActor* MyActor, const AActor* TargetActor) const
{
	float DistToBattleTarget = FVector::Dist(MyActor->GetActorLocation(),
		TargetActor->GetActorLocation());
	
	return MinAttackRange < DistToBattleTarget && DistToBattleTarget <= MaxAttackRange;
}

bool UBTDE_BattleMoveCondition::CheckTargetAngle(const AActor* MyActor, const AActor* TargetActor) const
{
	FVector MyForward = MyActor->GetActorForwardVector();
	FVector TargetLocation = (TargetActor->GetActorLocation() - MyActor->GetActorLocation()).GetSafeNormal2D();
	float AngleDeg = UKismetMathLibrary::DegAcos(FVector::DotProduct(MyForward, TargetLocation));

	return AngleDeg <= TargetAngle;
}