// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BaseAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UBaseAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	//~Begin UAnimNotify Interface.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	//~End UAnimNotify Interface.
};
