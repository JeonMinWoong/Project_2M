// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGA_ExecutionBase.h"
#include "TwoMinGA_ExecutionCaster.generated.h"

class UTwoMinAT_ExecutionAutoPosition;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_ExecutionCaster : public UTwoMinGA_ExecutionBase
{
	GENERATED_BODY()

public:
	FAttackInfoData& GetAttackInfoData();
	void AddComboCount(FGameplayEventData Payload);
	void InstantDamage(FGameplayEventData Payload);
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
		const FGameplayEventData* TriggerEventData = nullptr) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	virtual TSubclassOf<UGameplayEffect> GetAttackGameplayEffectClass() const override;
	
private:
	UAnimMontage* GetExecutionMontage(bool bIsExecutionForward, int32& ExecutionNumber);

	void StartAutoPosition();
	void ExecutionTargetAutoPosition(float DeltaTime, ATwoMinBaseCharacter* ExecutionTarget, AActor* MyActor,
		int32 ExecutionNumber, float SnapSpeed, FRotator& LookAtRot);
	void MyActorAutoPosition(float DeltaTime, ATwoMinBaseCharacter* ExecutionTarget, AActor* MyActor,
		float SnapSpeed, float SnapDistance, FRotator LookAtRot);

	UFUNCTION()
	void UpdateAutoPosition(float DeltaTime);

	UPROPERTY()
	UTwoMinAT_ExecutionAutoPosition* ExecutionAutoPositionTick;
	
	UPROPERTY(EditDefaultsOnly, Category = "ExecutionCaster|HitExecutionTag")
	FGameplayTag OnHitExecution;

	UPROPERTY(EditDefaultsOnly, Category = "ExecutionCaster|GameplayEffect")
	TSubclassOf<UGameplayEffect> AttackGameplayEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "ExecutionCaster|Forward|Montages")
	TMap<int32, UAnimMontage*> ExecutionForwardCasterMontages;
	
	UPROPERTY(EditDefaultsOnly, Category = "ExecutionCaster|Back|Montages")
	TMap<int32, UAnimMontage*> ExecutionBackCasterMontages;
	
	// 1 ~ 10 앞, 11 ~ 20 뒤
	UPROPERTY(EditDefaultsOnly, Category = "ExecutionCaster|ExecutionInfosData")
	TMap<int32, FExecutionInfoData> ExecutionInfosData;

	// 1 ~ 10 앞, 11 ~ 20 뒤
	UPROPERTY(EditDefaultsOnly, Category = "ExecutionCaster|AttackCount")
	TMap<int32, int32> MaxComboCount;

	// 1 ~ 10 앞, 11 ~ 20 뒤
	UPROPERTY(EditDefaultsOnly, Category = "ExecutionCaster|AttackCount")
	TMap<int32, int32> CurComboCount;

	UPROPERTY(EditDefaultsOnly, Category = "ExecutionCaster|bIsFinalDamageToInstantDeath")
	TMap<int32, bool> bIsFinalDamageToInstantDeath;
	
	UPROPERTY()
	UExecutionPayloadObject* CachedExecutionData;
};
