// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState/ANS_RotateDirection.h"
#include "ANS_RotateDirection_Player.generated.h"


class UAutoTargetingComponent;
enum class ERotateDirectionType : uint8;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UANS_RotateDirection_Player : public UANS_RotateDirection
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Setting", meta=(ClampMin="0", ClampMax="360", UIMin="0", UIMax="360"))
	float MaxRotationAngle;

	UPROPERTY(EditAnywhere, Category = "Setting|Type")
	ERotateDirectionType RotateDirectionType;
	
protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime,
	                        const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation,
		const FAnimNotifyEventReference& EventReference) override;
	//~ End UAnimNotifyState Interface.

	void CharacterToInputDirection(ATwoMinPlayerCharacter* PlayerCharacter);
	void CharacterToTargetDirection(ATwoMinPlayerCharacter* PlayerCharacter, float FrameDeltaTime);
	void CharacterToTargeting(ATwoMinPlayerCharacter* PlayerCharacter, const UAutoTargetingComponent* AutoTargetingComp,
		float FrameDeltaTime);
	AActor* GetLockOnTarget(const ATwoMinPlayerCharacter* PlayerCharacter);

private:
	UPROPERTY()
	bool bIsRotation;
	
};
