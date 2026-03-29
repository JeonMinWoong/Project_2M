// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "TwoMinBaseCharacter.h"
#include "TwoMinFunctionLibrary.h"
#include "Widgets/TwoMinWidget_WindowQuickSlot.h"
#include "TwoMinPlayerCharacter.generated.h"

class UNiagaraComponent;
class UInventoryComponent;
class UGameplayEffect;
class UPlayerUIComponent;
class ATwoMinEnemyCharacter;
class ATwoMinPlayerController;
struct FGameplayTag;
class UPlayerCombatComponent;
struct FInputActionValue;
class UDataAsset_InputConfig;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API ATwoMinPlayerCharacter : public ATwoMinBaseCharacter
{
	GENERATED_BODY()

public:
	ATwoMinPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual UBaseCombatComponent* GetCombatComponent() const override;
	virtual UBaseUIComponent* GetBaseUIComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUIComponent() const override;
	
	ATwoMinPlayerController* GetPlayerController() const;
	ATwoMinEnemyCharacter* GetCurrentAutoTarget() const;
	UInventoryComponent* GetInventoryComponent() const;
	
	void CancelInputToggle();
	FVector GetInputDirection() const;
	void MoveStop(const FInputActionValue& InputActionValue);
	bool CheckRunConditions();

	void PlayerLevelUp(int32 NewLevel);
	
	void OpenInventoryProcess();
	void OpenEndGameProcess();
	
	void PlayCameraShakeOnHit(ECameraShakeType CameraShakeType);

	void OnIgnoreInputProcess(bool bIsIgnore);
	bool IsPossibleUseItem();
	
	FTimerHandle FightDecreaseTimerHandle;
	
protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void AfterDeathProcess() override;
	void DefeatStateProcess();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	void CheckSpecialAttack(float DeltaTime);
	void CheckRunState(float DeltaTime);
	
	/** Components **/
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	UPlayerCombatComponent* PlayerCombatComponent;

	UPROPERTY(VisibleAnywhere)
	UPlayerUIComponent* PlayerUIComponent;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HUDOverlayClass;

	UPROPERTY()
	UUserWidget* HUDOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect|Experience_Gain")
	TSubclassOf<UGameplayEffect> ExperienceGainEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "CurveTable|NeedToLevelUp_Experience")
	UCurveTable* NeedToLevelUp_ExperienceCurveTable;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect|Gold_Gain")
	TSubclassOf<UGameplayEffect> GoldGainEffect;

	UPROPERTY(EditDefaultsOnly, Category = "CurveTable|Fight_Curve")
	UCurveTable* FightCurveTable;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect|Fight_Gain")
	TSubclassOf<UGameplayEffect> FightGainEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect|Fight_Decrease")
	TSubclassOf<UGameplayEffect> FightDecreaseEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect|Anger_Decrease")
	TSubclassOf<UGameplayEffect> AngerDecreaseEffect;
	
	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect|Status_Effect")
	TSubclassOf<UGameplayEffect> EquipStatusEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect|Status_Effect")
	TSubclassOf<UGameplayEffect> ConsumeStatusEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "NiagaraEffect|LevelUp|Effect")
	UNiagaraSystem* LevelUpEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "NiagaraEffect|LevelUp|Sound")
	USoundBase* LevelUpSound;
	
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComp;
	
	UPROPERTY()
	FTimerHandle LevelUpTimerHandle;
	
	UPROPERTY()
	AActor* ExecutionCinematicDummy;
	
	UPROPERTY()
	AActor* InteractionActor;
	
	UPROPERTY()
	bool bIsIgnoreInput = false;
	
	UPROPERTY()
	bool bIsEquipWeapon = false;
	
	/** CharacterInfo **/
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Capsule")
	float CapsuleRadius = 42.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Capsule")
	float CapsuleHalfHeight = 96.f;
	
	UPROPERTY(EditDefaultsOnly, Category= "CharacterInfo|Camera")
	float CameraBoomLength = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Camera")
	FVector SocketOffSet = FVector(0.f, 0, 80.f);

	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Camera")
	float CameraLagSpeed = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Camera")
	float CameraRotationLagSpeed = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Camera")
	bool bInvertPitch = false;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Camera")
	float FieldOfView = 90.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Camera|PC")
	float MouseSensitivityX = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Camera|PC")
	float MouseSensitivityY = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Camera|Pad")
	float GamepadSensitivityX = 0.8f;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|Camera|Pad")
	float GamepadSensitivityY = 0.8f;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|CharacterMovement")
	float MaxWalkSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|CharacterMovement")
	float MaxRunSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterInfo|CharacterMovement")
	FRotator CharacterRotationRate = FRotator(0.f, 500.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TMap<ECameraShakeType, TSubclassOf<UCameraShakeBase>> CameraShakeMap;
	
	UPROPERTY()
	UCameraShakeBase* CachedCameraShake;
	
	UPROPERTY()
	ECameraShakeType CachedCameraShakeType;
	
#pragma region Input
  	/** Inputs **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CharacterData", meta=(AllowPrivateAccess="true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	UPROPERTY(VisibleAnywhere)
	bool bIsWalk = false;
	
	UPROPERTY()
	bool bIsRunning = false;
	
	UPROPERTY()
	float InputRunningTime;
	
	UPROPERTY()
	float MaxInputRunningTime = 0.05f;
	
	UPROPERTY()
	TArray<FGameplayTag> MovePossibleCancelAbilityTags;
	
	UPROPERTY(EditDefaultsOnly, Category= "IgnoreTag|Move")
	FGameplayTagContainer MoveIgnoreTagContainer;
	
	UPROPERTY()
	FVector2D SwitchDirection = FVector2D::ZeroVector;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetTrigger(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetComplete(const FInputActionValue& InputActionValue);

	void Input_OnRun(const FInputActionValue& InputActionValue);

	bool IsUsingGamepad() const;

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	
	void Input_ObjectiveTargetingTrigger(const FInputActionValue& InputActionValue);
	void Input_PickUpTrigger(const FInputActionValue& InputActionValue);
	void Input_InteractTrigger(const FInputActionValue& InputActionValue);
	
	UPROPERTY(EditDefaultsOnly, Category= "IgnoreTag|Run")
	FGameplayTagContainer RunIgnoreTagContainer;
	
	UPROPERTY(EditDefaultsOnly, Category= "IgnoreTag|ItemPickUpTrigger")
	FGameplayTagContainer IgnoreTagContainer;

	void Input_OpenInventory(const FInputActionValue& InputActionValue);
	
	bool bIsOpenInventory = false;
	
	void Input_UseItemTrigger(const FInputActionValue& InputActionValue);
	
	void Input_LeftQuickSlotTrigger(const FInputActionValue& InputActionValue);
	void Input_RightQuickSlotItemTrigger(const FInputActionValue& InputActionValue);
	
	void Input_SpecialAttack_Check_Trigger(const FInputActionValue& InputActionValue);
	
	bool bIsSpecialAttackCheck = false;
	float InputDelay = 0.1f;
	float CurDelay = 0.f;
	
	void Input_EndGameTrigger(const FInputActionValue& InputActionValue);
	
	bool bIsEndGameTrigger = false;
	
#pragma endregion

public:
	bool GetIsRunning();
	FORCEINLINE UCameraComponent* GetCamera() const { return CameraComponent; }
	
	FORCEINLINE void SetExecutionCinematicDummy(AActor* FoundExecutionCam) { ExecutionCinematicDummy = FoundExecutionCam; }
	FORCEINLINE AActor* GetExecutionCinematicDummy() const { return ExecutionCinematicDummy; }

	FORCEINLINE void SetInteractionActor(AActor* NewInteractionActor) { InteractionActor = NewInteractionActor; }
	FORCEINLINE AActor* GetInteractionActor() const { return InteractionActor; }

	
	FORCEINLINE void AddMovePossibleCancelAbility(const FGameplayTag GameplayTag)
	{ MovePossibleCancelAbilityTags.AddUnique(GameplayTag); }
	
	FORCEINLINE void RemoveMovePossibleCancelAbility(const FGameplayTag GameplayTag)
	{ MovePossibleCancelAbilityTags.Remove(GameplayTag); }

	FORCEINLINE UGameplayEffect* GetExperienceGainEffect() const
	{ return ExperienceGainEffect->GetDefaultObject<UGameplayEffect>(); }

	FORCEINLINE UCurveTable* GetNeedToLevelUp_ExperienceCurveTable() const
	{ return NeedToLevelUp_ExperienceCurveTable; }
	
	FORCEINLINE UGameplayEffect* GetGoldGainEffect() const
	{ return GoldGainEffect->GetDefaultObject<UGameplayEffect>(); }
	
	FORCEINLINE UCurveTable* GetFightCurveTable() const
	{ return FightCurveTable; }
	
	FORCEINLINE UGameplayEffect* GetFightGainEffect() const
	{ return FightGainEffect->GetDefaultObject<UGameplayEffect>(); }
	
	FORCEINLINE UGameplayEffect* GetFightDecreaseEffect() const
	{ return FightDecreaseEffect->GetDefaultObject<UGameplayEffect>(); }
	
	FORCEINLINE UGameplayEffect* GetAngerDecreaseEffect() const
	{ return AngerDecreaseEffect->GetDefaultObject<UGameplayEffect>(); }
	
	FORCEINLINE TSubclassOf<UGameplayEffect> GetEquipStatusEffect() const
	{ return EquipStatusEffect; }
	
	FORCEINLINE TSubclassOf<UGameplayEffect> GetConsumeStatusEffect() const
	{ return ConsumeStatusEffect; }
	
	FORCEINLINE UUserWidget* GetHUDOverlay() const { return HUDOverlay; }
	
	FORCEINLINE void SetEquipWeapon(bool bOn) { bIsEquipWeapon = bOn; }
	FORCEINLINE bool GetEquipWeapon() const { return bIsEquipWeapon; };
};

