// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "TwoMinGA_LockOn_Player.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UInputMappingContext;
class UTwoMinAT_LockOn_Player;
class UTwoMinWidgetBase;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinGA_LockOn_Player : public UTwoMinGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

private:
	AActor* FindLockOnTarget();
	void CheckSphereOverTargetGroup();

	bool IsLockOnCondition(const AActor* Target) const;
	bool IsLockOnDistance(const FVector& PlayerLocation, const FVector& TargetLocation) const;
	bool IsLockOnLineTraceHit(FVector CameraLocation, FVector TargetLocation) const;
	bool IsLockOnScreenToDistance(const AActor* Target, float& ClosestDistance) const;
	bool IsInScreenPos(const AActor* Target, FVector2D& ScreenCenter, FVector2D& ScreenPos) const;
	
	void DrawLockOnWidget();
	void SetTargetLockOnWidgetPosition();
	void LockCharacterMovement();
	void StartLockOnTickTask();
	void ChangeMappingContext();
	
	UFUNCTION()
	void UpdateLockOnTarget(float DeltaTime);

	UFUNCTION()
	void OnSwitchTarget(FGameplayEventData InputEventData);
	void OnSplitLeftRightTargetGroup(TArray<AActor*>& ActorsOnLeft, TArray<AActor*>& ActorsOnRight);

	void EndLockOnTarget();
	void ResetMappingContext();
	void ResetCharacterMovement();
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float LockOnDistance = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings",
		meta=(ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float LockOnAngle = 45.f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float LockOnSwitchTime = 0.5f;

	UPROPERTY()
	float CurrentSwitchTime;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings|OffsetDistance")
	float LockOnCameraOffsetDistance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|LockSpeed")
	float LockOnCameraRotationSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|Character")
	float LockOnCharacterRotationSpeed = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Debug")
	bool bIsDebugLockOnRange = false;
	
	UPROPERTY()
	bool bIsCharacterRotationLock = false;
	
	UPROPERTY()
	TArray<AActor*> LockOnTargetGroup;

	UPROPERTY()
	AActor* LockOnTarget;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|Image")
	TSubclassOf<UTwoMinWidgetBase> LockOnTargetWidgetClass;

	UPROPERTY()
	UTwoMinWidgetBase* LockOnTargetWidget;

	UPROPERTY()
	FVector2D LockOnTargetWidgetSize = FVector2D::ZeroVector;

	UPROPERTY()
	UTwoMinAT_LockOn_Player* LockOnTickTask;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* LockOnSwitchTargetEvent;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|Mapping")
	UInputMappingContext* LockOnInputMappingContext;
};
