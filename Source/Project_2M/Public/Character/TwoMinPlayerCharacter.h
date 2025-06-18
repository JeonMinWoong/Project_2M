// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TwoMinBaseCharacter.h"
#include "TwoMinPlayerCharacter.generated.h"

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
	ATwoMinPlayerCharacter();

	virtual UBaseCombatComponent* GetCombatComponent() const override;
	
protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	/** Components **/
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	UPlayerCombatComponent* PlayerCombatComponent;
	
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

#pragma region Input
	
  	/** Inputs **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CharacterData", meta=(AllowPrivateAccess="true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	UPROPERTY(VisibleAnywhere)
	bool bIsWalk = false;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsRun = false;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Stoped(const FInputActionValue& InputActionValue);

	void Input_ToggleRun(const FInputActionValue& InputActionValue);

	bool IsUsingGamepad() const;

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	
#pragma endregion

public:
	FORCEINLINE bool GetIsRunning() const { return bIsRun; }
	FORCEINLINE UCameraComponent* GetCamera() const { return CameraComponent; }
};

