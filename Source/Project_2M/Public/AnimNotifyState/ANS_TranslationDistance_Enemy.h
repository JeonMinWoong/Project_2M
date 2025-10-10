// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState/ANS_TranslationDistance.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "ANS_TranslationDistance_Enemy.generated.h"

class ATwoMinEnemyCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_TranslationDistance_Enemy : public UANS_TranslationDistance
{
	GENERATED_BODY()

protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;
	//~ End UAnimNotifyState Interface.
	
private:	
	void CharacterToBattleTargetDistance(ATwoMinEnemyCharacter* EnemyCharacter, const float FrameDeltaTime);
	AActor* GetLockOnTarget(const ATwoMinEnemyCharacter* EnemyCharacter);

	UPROPERTY(EditAnywhere, Category = "BBKey")
	FName BlackboardKeyName = NAME_None;
	
	UPROPERTY(EditAnywhere, Category = "Setting|Data")
	FTwoMinEnemyAttackApproachData AttackApproachData;

};
