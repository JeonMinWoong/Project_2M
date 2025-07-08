// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TwoMinGameplayTag.h"
#include "AnimNotifyState/BaseAnimNotifyState.h"
#include "ANS_ApplyStateTag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_ApplyStateTag : public UBaseAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "StateTag", meta = (Categories = "Shard_State"))
	FGameplayTag StateTagToApply;
	
protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation,
		const FAnimNotifyEventReference& EventReference) override;
	//~ End UAnimNotifyState Interface.
};
