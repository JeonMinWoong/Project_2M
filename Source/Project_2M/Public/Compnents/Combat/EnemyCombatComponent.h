// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Engine/TargetPoint.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "EnemyCombatComponent.generated.h"

class ATwoMinBaseCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UEnemyCombatComponent : public UBaseCombatComponent
{
	GENERATED_BODY()

public:
	UEnemyCombatComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
								   FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void OnHitTargetActor(AActor* HitActor) override;

#pragma region Battle
	bool IsAttackCondition(const AActor* TargetActor, int AttackConditionIndex) const;
	bool IsEvenOneAttackCondition(const AActor* TargetActor) const;
	bool IsEvenOneAttackCooldown();
	bool IsStopBattleMoveGameplayContainer();
	bool IsEvasionCondition(AActor* TargetActor, EEnemyEvasionType EvasionType) const;
	
	void UpdateBanAttack(float MinBanAttackDelay, float MaxBanAttackDelay);
	void ClearBanAttack();
	
	UPROPERTY(EditDefaultsOnly, Category = "Battle|AttackConditions")
	TMap<int, FEnemyAIAttackConditionData> AttackConditions;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|StopBattleMoveTags")
	FGameplayTagContainer StopBattleTags;
	
	UPROPERTY(EditAnywhere, Category = "Battle|EvasionConditions")
	TMap<EEnemyEvasionType, FEnemyAIEvasionConditionDate> EvasionConditions;
	
#pragma endregion
	
#pragma region Patrol
	
	int32 GetNextPatrolPointIndex();

	void ResetPatrolPoint();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPoints")
	TArray<FEnemyAIPatrolData> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolMode")
	EPatrolPathMode PatrolPathMode = EPatrolPathMode::Loop;

	UPROPERTY(VisibleAnywhere)
	bool bIsPrevPatrolPoint = false;
	
#pragma endregion
	
private:

#pragma region Battle

	bool bIsBattlePossible = false;
	
	bool bIsBanAttack = false;
	
	FTimerHandle BanAttackTimerHandle;
	
#pragma endregion

#pragma region Detect

	UPROPERTY(EditAnywhere, Category = "WeaponEquipPossible")
	bool bIsEquipPossible = false;
	
	bool bIsEquip = false;
	
#pragma endregion
	
#pragma region Patrol
	
	UFUNCTION()
	void PatrolWaitPoint();
	
	UPROPERTY(VisibleAnywhere, Category = "IsPatrol")
	bool bIsPatrol;

	UPROPERTY(VisibleAnywhere, Category = "CurrentPatrolPoint")
	int32 CurrentPatrolPointIndex = 0;

	bool bPingPongForward = true;
	bool bIsPatrolPointArrivedWait = false;
	float CurrentPatrolPointArrivedWaitTime = 0.f;
	
#pragma endregion
	
public:
	FORCEINLINE bool IsBattlePossible() const { return bIsBattlePossible; }
	FORCEINLINE void SetIsBattlePossible(bool InIsBattlePossible) { bIsBattlePossible = InIsBattlePossible; }
	
	FORCEINLINE bool IsBanAttack() const { return bIsBanAttack; }
	
	FORCEINLINE bool IsEquip() const { return bIsEquipPossible ? bIsEquip : false; }
	FORCEINLINE void SetIsEquip(bool InIsEquip)
	{
		if (bIsEquipPossible == false)return;
		bIsEquip = InIsEquip;
	}
	
	FORCEINLINE bool IsPatrol() const { return bIsPatrol; }
	FORCEINLINE void SetIsPatrol(bool InIsPatrol) { bIsPatrol = InIsPatrol; }
	FORCEINLINE int32 GetCurrentPatrolPointIndex() const { return CurrentPatrolPointIndex; }
	FORCEINLINE void ResetCurrentPatrolPointIndex() { CurrentPatrolPointIndex = 0; }
};
