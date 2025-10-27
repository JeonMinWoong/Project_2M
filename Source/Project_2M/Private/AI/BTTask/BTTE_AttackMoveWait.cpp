// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTE_AttackMoveWait.h"

#include "AIController.h"
#include "TwoMinFunctionLibrary.h"

UBTTE_AttackMoveWait::UBTTE_AttackMoveWait()
{
	bNotifyTick = true;
}

FString UBTTE_AttackMoveWait::GetStaticDescription() const
{
	return FString::Printf(TEXT("ToTalWaitTime %s"), *FString::SanitizeFloat(TotalWaitTime));
}

EBTNodeResult::Type UBTTE_AttackMoveWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	MaxRange = FMath::Max(MinRange, MaxRange);
	TotalWaitTime = FMath::RandRange(MinRange, MaxRange);
	ElapsedTime = 0;
	return EBTNodeResult::InProgress;
}

void UBTTE_AttackMoveWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ElapsedTime += DeltaSeconds;

	if (CanAttackMoveWait(OwnerComp))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	if (ElapsedTime >= TotalWaitTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

bool UBTTE_AttackMoveWait::CanAttackMoveWait(UBehaviorTreeComponent& OwnerComp) const
{
	AActor* MyActor = Cast<AActor>(OwnerComp.GetAIOwner()->GetPawn());
	if (CheckAttackCooldownTags.IsEmpty())
	{
		return false;
	}

	bool bCanAttack = false;
	for (auto CheckAttackCooldownTag : CheckAttackCooldownTags)
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(MyActor, CheckAttackCooldownTag) == false)
		{
			bCanAttack = true;
		}
	}
	
	return bCanAttack;
}
