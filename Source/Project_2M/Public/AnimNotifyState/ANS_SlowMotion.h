// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState/BaseAnimNotifyState.h"
#include "ANS_SlowMotion.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_SlowMotion : public UBaseAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "SlowMotionRate")
	float SlowMotionRate;
	
protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation,
		const FAnimNotifyEventReference& EventReference) override;
	//~ End UAnimNotifyState Interface.
	
private:
	float OriginalSlowMotionRate = 1.f;

};
