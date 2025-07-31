// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState/ANS_TA_CancelAndStartNewAction.h"
#include "ANS_StartConversionCombo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_StartConversionCombo : public UANS_TA_CancelAndStartNewAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "ConversionComboCount")
	int32 ConversionComboCount = 1;

protected:
	virtual void NewAbilityCustomStartProcess(const ATwoMinBaseCharacter* Owner, UTwoMinGameplayAbility* NewAbility) override;
	virtual void NewAbilityCustomEndProcess(const ATwoMinBaseCharacter* Owner, UTwoMinGameplayAbility* NewAbility) override;
	
};
