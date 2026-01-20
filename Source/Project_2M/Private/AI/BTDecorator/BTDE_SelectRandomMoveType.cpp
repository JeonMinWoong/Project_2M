// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDE_SelectRandomMoveType.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

UBTDE_SelectRandomMoveType::UBTDE_SelectRandomMoveType()
{
	NodeName = "Random Move Type";
}

bool UBTDE_SelectRandomMoveType::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	int32 RandomIndex = FMath::RandRange(0, MoveTypeToList.Num() - 1);
	EEnemyAIMoveType SelectedMoveType = MoveTypeToList[RandomIndex];
	
	UBlackboardComponent* BB = GetBlackboardComponent(OwnerComp);
	if (!BB) return false;
	
	BB->SetValueAsEnum(TwoMinBBKeys::SelectMoveType, static_cast<uint8>(SelectedMoveType));
	return SelectedMoveType != EEnemyAIMoveType::None;
}
