// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/ANS_TranslationDistance_Player.h"

#include "Character/TwoMinPlayerCharacter.h"

void UANS_TranslationDistance_Player::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	bIsTranslation = false;
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UANS_TranslationDistance_Player::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (bIsTranslation)
	{
		return;
	}

	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacter)
	{
		return;
	}

	FVector CurrentLocation = PlayerCharacter->GetActorLocation();
	FVector InputDirection = PlayerCharacter->GetInputDirection().GetSafeNormal();
	FVector TargetLocation = CurrentLocation + (InputDirection * TranslationDistance);

	bIsTranslation = true;
	PlayMotionWarpingTargetLocation(PlayerCharacter, TargetLocation);
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UANS_TranslationDistance_Player::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	bIsTranslation = false;
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
