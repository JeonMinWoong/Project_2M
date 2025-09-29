// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState/ANS_RotateDirection.h"
#include "ANS_RotateDirection_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_RotateDirection_Enemy : public UANS_RotateDirection
{
	GENERATED_BODY()

protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;
	//~ End UAnimNotifyState Interface.

	virtual AActor* GetLockOnTarget(ATwoMinBaseCharacter* MyActor) override;
};
