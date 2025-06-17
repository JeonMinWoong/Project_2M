// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState/ANS_ToggleAbilityBase.h"
#include "ANS_TA_ComboStartAndEnd.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_TA_ComboStartAndEnd : public UANS_ToggleAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void StartToggleAbilityProcess(UTwoMinGameplayAbility* InAbility) override;
	virtual void EndToggleAbilityProcess(UTwoMinGameplayAbility* InAbility) override;
};
