// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState/ANS_ToggleAbilityBase.h"
#include "ANS_TA_CancelAndStartNewAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_TA_CancelAndStartNewAction : public UANS_ToggleAbilityBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "NewAbilityTag")
	FGameplayTag NewAbilityTag;
	
protected:
	virtual void StartToggleAbilityProcess(UTwoMinGameplayAbility* InAbility) override;
	virtual void EndToggleAbilityProcess(UTwoMinGameplayAbility* InAbility) override;
	
};
