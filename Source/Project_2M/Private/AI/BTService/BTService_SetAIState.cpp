// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_SetAIState.h"

#include "BehaviorTree/BlackboardComponent.h"

void UBTService_SetAIState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;
	
	BB->SetValueAsString(OutTargetActorKey.SelectedKeyName, AIStateString);
}
