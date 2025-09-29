// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState/BaseAnimNotifyState.h"
#include "ANS_RotateDirection.generated.h"

class ATwoMinBaseCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_RotateDirection : public UBaseAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotateSpeed")
	float LockTargetRotateSpeed = 10.f;
	
protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation,
		const FAnimNotifyEventReference& EventReference) override;
	//~ End UAnimNotifyState Interface.

	virtual void CharacterToTargetDirection(ATwoMinBaseCharacter* MyActor, float FrameDeltaTime);
	virtual AActor* GetLockOnTarget(ATwoMinBaseCharacter* MyActor);
	
	virtual void PlayMotionWarpingRotator(ATwoMinBaseCharacter* Character, FRotator TargetDRotator);

	UPROPERTY()
	bool bIsRotation;
	
};
