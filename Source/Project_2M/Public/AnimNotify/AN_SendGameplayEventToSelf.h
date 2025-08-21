// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AnimNotify/BaseAnimNotify.h"
#include "AN_SendGameplayEventToSelf.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UAN_SendGameplayEventToSelf : public UBaseAnimNotify
{
	GENERATED_BODY()

protected:
	//~Begin UAnimNotify Interface.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	//~End UAnimNotify Interface.

private:
	UPROPERTY(EditAnywhere, Category = "SendGameplayEventToSelf")
	FGameplayTag EventTag;
};
