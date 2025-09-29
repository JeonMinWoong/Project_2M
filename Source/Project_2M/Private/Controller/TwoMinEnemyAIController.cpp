// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/TwoMinEnemyAIController.h"

#include "TwoMinDebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

ATwoMinEnemyAIController::ATwoMinEnemyAIController()
{
	AAIController::SetGenericTeamId(FGenericTeamId(1));

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 5000.f;
	AISenseConfig_Sight->LoseSightRadius = 6000.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 180.f;
	AISenseConfig_Sight->AutoSuccessRangeFromLastSeenLocation = 0.f;

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);
}

ETeamAttitude::Type ATwoMinEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent =
		Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void ATwoMinEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void ATwoMinEnemyAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor) return;
	
	UBlackboardComponent* BB = GetBlackboardComponent();
	UAIPerceptionComponent* Perception = FindComponentByClass<UAIPerceptionComponent>();
	if (!BB || !Perception) return;

	const FAISenseID SightID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	if (Stimulus.Type != SightID)
	{
		return;
	}
	
	if (Stimulus.WasSuccessfullySensed())
	{
		BB->SetValueAsObject(TwoMinBBKeys::TargetActor, Actor);
		return;
	}

	AActor* Cur = Cast<AActor>(BB->GetValueAsObject(TwoMinBBKeys::TargetActor));
	if (!Stimulus.WasSuccessfullySensed() && Cur)
	{
		BB->ClearValue(TwoMinBBKeys::TargetActor);
	}
}