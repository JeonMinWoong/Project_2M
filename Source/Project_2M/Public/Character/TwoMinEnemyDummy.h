// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "TwoMinEnemyDummy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinEnemyDummy : public ATwoMinEnemyCharacter
{
	GENERATED_BODY()

public:
	ATwoMinEnemyDummy(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Dummy|AttackIndex")
	int32 AttackIndex = 1;

	UPROPERTY(EditAnywhere, Category = "Dummy|AutoAction")
	bool bAutoAction = false;

	UPROPERTY(EditAnywhere, Category = "Dummy|AutoAction")
	bool bResetAttackLocation = false;
	
	UPROPERTY(EditAnywhere, Category = "Dummy|IsUseHitReact")
	bool bIsUseHitReact = true;
	
	UPROPERTY(EditAnywhere, Category = "Dummy|IsUsePush")
	bool bIsUsePush = true;
	
	UPROPERTY(EditAnywhere, Category = "Dummy|IsPossibleExecution")
	bool bIsPossibleExecution = false;
	
	UPROPERTY(VisibleAnywhere, Category= "UI")
	UWidgetComponent* AIStateWidgetComponent;
	
public:
	FORCEINLINE int32 GetAttackIndex() const { return AttackIndex; }
	FORCEINLINE bool GetAutoAction() const { return bAutoAction; }
	FORCEINLINE bool GetResetAttackLocation() const { return bResetAttackLocation; }
	FORCEINLINE bool GetIsUseHitReact() const { return bIsUseHitReact; }
	FORCEINLINE bool GetIsUsePush() const { return bIsUsePush; }
	FORCEINLINE bool GetIsPossibleExecution() const { return bIsPossibleExecution; }
};
