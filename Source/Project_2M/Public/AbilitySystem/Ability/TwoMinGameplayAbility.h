// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "TwoMinGameplayAbility.generated.h"

class ULevelSequencePlayer;
class ULevelSequence;
class UAbilityTask_PlayMontageAndWait;
class UMotionWarpingComponent;
struct FAttackInfoData;
class ATwoMinBaseCharacter;
class FWaitGameplayEventDelegate;

UENUM(BlueprintType)
enum class EToMinAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven,
};

UENUM(BlueprintType)
enum class ETwoAbilityInputType : uint8
{
	Only,
	ReTriggerable,
};


/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UTwoMinGameplayAbility();

	void SetReTriggerActive(bool bIsReTriggerActive);

	void AddPossibleCancelAbility(UTwoMinGameplayAbility* InAbility);
	void RemovePossibleCancelAbility(UTwoMinGameplayAbility* InAbility);
	bool IsPossibleCancelAbility(UTwoMinGameplayAbility* InAbility) const;
	virtual void CustomCancelAbility();

	virtual bool IsMustBeLikedToGameplayAbility() const;
	virtual bool IsPossibleMustBeHoldAbilityImmediatelyCancel() const;
	
	void OnHitStop(const FGameplayEventData& Payload, bool bIsTargetGuard, const UAttackPayloadObject* AttackPayload,
		ATwoMinBaseCharacter* TargetCharacter);
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UTwoMinAbilitySystemComponent* GetTwoMinAbilitySystemComponentFromActorInfo() const;
	
	virtual bool bIsReTriggerSameAbility() const;
	
	void PlayLevelSequence(ATwoMinBaseCharacter* OwnerCharacter, ULevelSequence* LevelSequence, float BlendDelay);
	
	UFUNCTION()
	void OnFinishLevelSequence();
	
	UAbilityTask_PlayMontageAndWait* PlayToAnimMontage(UAnimMontage* AnimMontage, FName StartSectionName = NAME_None,
		bool bStopWhenAbilityEnds = false, bool bIsBlendOutCancel = false);
	virtual void WaitGameplayEvent(FGameplayTag EventTag, bool bIsOnce = false);
	
	void OnStartKnockBack(AActor* OwnerActor, UAnimMontage* TargetMontage, const FVector& Direction,
		const float PushDistance, const float EndTime, UCurveFloat* KnockBackCurve);
	
	UFUNCTION()
	virtual void CustomEventReceived(FGameplayEventData Payload);
	
	UFUNCTION()
	virtual void OnAttackGameplayEventReceivedByMelee(FGameplayEventData Payload);  

	UFUNCTION()
	virtual void OnAttackGameplayEventReceivedByRange(FGameplayEventData Payload);

	UFUNCTION()
	virtual void OnExecutionGameplayEventReceive(FGameplayEventData Payload);
	
	UFUNCTION()
	virtual void OnResetAttackCountGameplayEffectReceive(FGameplayEventData Payload);
	
	UFUNCTION()
	virtual void OnAttackGameplayEventReceivedByLocation(FGameplayEventData Payload);  
	
	UFUNCTION()
	virtual void OnAbilityGameplayEventReceivedByTarget(FGameplayEventData Payload);
	
	UFUNCTION()
	virtual void CustomCompleteAbility();
	
	UFUNCTION()
	virtual void CustomInterruptedAbility();

	UFUNCTION()
	virtual void CustomOnBlendOutAbility();

	void MeleeOnHitEffectProcess(ATwoMinBaseCharacter* OwnerCharacter, bool bIsTargetGuard, int32 WeaponIndex);
	
	UFUNCTION()
	virtual void DamageToEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, FGameplayEventData Payload,
		bool bIsTargetGuard, bool bIsExecution = false);

	FGameplayTag GetHitGameplayEffectTag(bool bIsTargetGuard, EHitType HitType);

	void LookDirectionHitPos(const FGameplayEventData* TriggerEventData);
	
	void HitStopProcess(AActor* HitStopCharacter, const float HitStopDelay);
	
	void CameraShakeToShakeType(AActor* ShakeCharacter, ECameraShakeType CameraShakeType);
	void CameraShakeOnHitReact(AActor* ShakeCharacter, EHitType HitType, bool bIsTargetGuard);
	
	virtual TSubclassOf<UGameplayEffect> GetAttackGameplayEffectClass() const;

	float CalculationStaminaCost() const;

	virtual float AddStaminaCost() const;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                       const FGameplayAbilityActivationInfo ActivationInfo) const override;

	void SendToExhaustedEvent() const;
	
	void EnableHitCollision(ATwoMinBaseCharacter* BaseCharacter, FVector TargetLocation = FVector::ZeroVector);
	
	UPROPERTY(EditDefaultsOnly, Category = "AbilityPoicy")
	EToMinAbilityActivationPolicy AbilityActivationPolicy = EToMinAbilityActivationPolicy::OnTriggered;
	
	UPROPERTY(EditDefaultsOnly, Category= "Cost")
	UCurveTable* StaminaCostCurveTable;

	UPROPERTY(EditDefaultsOnly, Category="CostName")
	FName StaminaCostName;
	
	UPROPERTY()
	ETwoAbilityInputType AbilityInputType = ETwoAbilityInputType::Only;

	UPROPERTY()
	bool bIsReTriggerAble = false;

	UPROPERTY()
	mutable bool bIsEndAbilitySendToExhaustedEvent = false;
	
	UPROPERTY()
	ULevelSequencePlayer* LevelSequencePlayer;
	
	UPROPERTY()
	float OriginCamConvertBlendDelay;

private:
	
	bool CalcKnockbackTarget(ATwoMinBaseCharacter* Char, const FVector& Dir, float Distance, FVector& OutTarget);
	void SetCurveRootMotion(UAnimMontage* TargetMontage, UMotionWarpingComponent* MW, const float StartTime,
		const float EndTime, UCurveFloat* KnockBackCurve);
	
	UPROPERTY()
	TArray<UTwoMinGameplayAbility*> PossibleCancelAbilities;
	
	UPROPERTY()
	ATwoMinBaseCharacter* CachedAbilityTargetCharacter;

public:
	FORCEINLINE ETwoAbilityInputType GetAbilityInputType() const { return AbilityInputType; }
	FORCEINLINE EToMinAbilityActivationPolicy GetActivationPolicy() const { return AbilityActivationPolicy; }
	FORCEINLINE bool IsReTriggerActive() const { return bIsReTriggerAble; }
	FORCEINLINE ATwoMinBaseCharacter* GetCachedAbilityTargetCharacter() const { return CachedAbilityTargetCharacter; }
};
