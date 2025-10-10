// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState/ANS_TranslationDistance.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "ANS_TranslationDistance_Player.generated.h"

class UAutoTargetingComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_TranslationDistance_Player : public UANS_TranslationDistance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Setting|Type")
	ETranslationDistanceType TranslationDistanceType;

	UPROPERTY(EditAnywhere, Category = "Setting|Data")
	FTwoMinPlayerAttackApproachData AttackApproachData;
	
protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime,
	                        const FAnimNotifyEventReference& EventReference) override;
	//~ End UAnimNotifyState Interface.

	void CharacterToInputDistance(ATwoMinPlayerCharacter* PlayerCharacter);
	void CharacterToAutoTargetingDistance(ATwoMinPlayerCharacter* PlayerCharacter,
		const UAutoTargetingComponent* AutoTargetingComp, const float FrameDeltaTime);
	AActor* GetLockOnTarget(const ATwoMinPlayerCharacter* PlayerCharacter);
};
