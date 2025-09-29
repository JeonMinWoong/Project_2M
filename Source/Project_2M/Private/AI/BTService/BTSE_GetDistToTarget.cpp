// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTSE_GetDistToTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Controller/TwoMinEnemyAIController.h"


void UBTSE_GetDistToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyCharacter) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	UObject* ActorObject = BB->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	if (!ActorObject) return;
	
	ATwoMinBaseCharacter* TargetActor = Cast<ATwoMinBaseCharacter>(ActorObject);

	float DistToTarget = FVector::Dist(EnemyCharacter->GetActorLocation(), TargetActor->GetActorLocation());
	BB->SetValueAsFloat(OutTargetActorKey.SelectedKeyName, DistToTarget);
}
