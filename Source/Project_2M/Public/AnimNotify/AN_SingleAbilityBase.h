// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AnimNotify/BaseAnimNotify.h"
#include "AN_SingleAbilityBase.generated.h"

class UTwoMinGameplayAbility;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UAN_SingleAbilityBase : public UBaseAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "AbilityTag")
	FGameplayTag AbilityTag;
	
protected:
	//~Begin UAnimNotify Interface.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	//~End UAnimNotify Interface.

	virtual void FinishAbilityProcess(UTwoMinGameplayAbility* InAbility);
};
