// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_TranslationDistance_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Controller/TwoMinEnemyAIController.h"

void UANS_TranslationDistance_Enemy::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (bIsTranslation) return;
	
	ATwoMinEnemyCharacter* MyActor = Cast<ATwoMinEnemyCharacter>(MeshComp->GetOwner());
	if (!MyActor) return;
	
	CharacterToBattleTargetDistance(MyActor, FrameDeltaTime);
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_TranslationDistance_Enemy::CharacterToBattleTargetDistance(ATwoMinEnemyCharacter* EnemyCharacter,
                                                                     const float FrameDeltaTime)
{
	const AActor* Target = GetLockOnTarget(EnemyCharacter);
	if (!Target) return;

	const FVector PlayerLocation = EnemyCharacter->GetActorLocation();
	const FVector TargetLocation = Target->GetActorLocation();
	float DistanceToTarget = FVector::Dist(PlayerLocation, TargetLocation);
	if (DistanceToTarget > TranslationDistance || DistanceToTarget <= AttackApproachData.ReachDistance)
	{
		bIsTranslation = true;
		return;
	}
	
	const FRotator TargetDirection = (Target->GetActorLocation() - EnemyCharacter->GetActorLocation()).Rotation();
	const FVector NewLocation = FMath::VInterpTo(PlayerLocation,
		TargetLocation - TargetDirection.Vector() * AttackApproachData.ReachDistance,
		FrameDeltaTime, AttackApproachData.ApproachSpeed);
	
	EnemyCharacter->SetActorLocation(NewLocation);
}

AActor* UANS_TranslationDistance_Enemy::GetLockOnTarget(const ATwoMinEnemyCharacter* EnemyCharacter)
{
	if (!EnemyCharacter) return nullptr;

	ATwoMinEnemyAIController* AIController = Cast<ATwoMinEnemyAIController>(EnemyCharacter->GetController());
	if (!AIController) return nullptr;

	UObject* Object = AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeyName);
	if (!Object) return nullptr;

	ATwoMinBaseCharacter* Target = Cast<ATwoMinBaseCharacter>(Object);
	if (!Target) return nullptr;
	
	return Target;
}
