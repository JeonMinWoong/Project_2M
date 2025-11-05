// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTSE_LookDirection.h"

#include "AIController.h"
#include "TwoMinDebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTSE_LookDirection::UBTSE_LookDirection()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
	
	RotationSpeed = 5.f;
	Interval = 0.f;
	RandomDeviation = 0.f;

	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey),
		AActor::StaticClass());
}

void UBTSE_LookDirection::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTSE_LookDirection::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (OwningPawn && TargetActor)
	{
		const FRotator LookAtRot = (TargetActor->GetActorLocation() - OwningPawn->GetActorLocation()).Rotation();
		const FRotator NewCharacterRot = FMath::RInterpTo(OwningPawn->GetActorRotation(),
	FRotator(0.f, LookAtRot.Yaw, 0.f), DeltaSeconds, RotationSpeed);
		
		OwningPawn->SetActorRotation(NewCharacterRot);
	}
}
