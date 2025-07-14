// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AnimNotifyState/BaseAnimNotifyState.h"
#include "ANS_ToggleAbilityBase.generated.h"

class UTwoMinGameplayAbility;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_ToggleAbilityBase : public UBaseAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "AbilityTag")
	FGameplayTag PlayingAbilityTag;
	
protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation,
		const FAnimNotifyEventReference& EventReference) override;
	//~ End UAnimNotifyState Interface.

	virtual void StartToggleAbilityProcess(UTwoMinGameplayAbility* InAbility);
	virtual void EndToggleAbilityProcess(UTwoMinGameplayAbility* InAbility);
};
