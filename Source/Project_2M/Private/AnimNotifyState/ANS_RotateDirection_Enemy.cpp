// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_RotateDirection_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Controller/TwoMinEnemyAIController.h"


void UANS_RotateDirection_Enemy::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (bIsRotation) return;
	
	ATwoMinBaseCharacter* MyActor = Cast<ATwoMinBaseCharacter>(MeshComp->GetOwner());
	if (!MyActor) return;
	
	CharacterToTargetDirection(MyActor, FrameDeltaTime);
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

AActor* UANS_RotateDirection_Enemy::GetLockOnTarget(ATwoMinBaseCharacter* MyActor)
{
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(MyActor);
	if (!EnemyCharacter) return nullptr;

	ATwoMinEnemyAIController* AIController = Cast<ATwoMinEnemyAIController>(EnemyCharacter->GetController());
	if (!AIController) return nullptr;

	UObject* Object = AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeyName);
	if (!Object) return nullptr;

	ATwoMinBaseCharacter* Target = Cast<ATwoMinBaseCharacter>(Object);
	if (!Target) return nullptr;
	
	return Target;
}
