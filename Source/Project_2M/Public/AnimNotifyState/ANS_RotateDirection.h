// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState/BaseAnimNotifyState.h"
#include "ANS_RotateDirection.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_RotateDirection : public UBaseAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Setting")
	float RotationSpeed;

protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;
	//~ End UAnimNotifyState Interface.
};
