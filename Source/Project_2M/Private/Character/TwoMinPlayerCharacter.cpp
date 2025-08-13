// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TwoMinPlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TwoMinGameplayTag.h"
#include "EnhancedInputSubsystems.h"
#include "TwoMinFunctionLibrary.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Compnents/AutoTargetingComponent.h"
#include "Compnents/Combat/PlayerCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controller/TwoMinPlayerController.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/CharacterInputComponent.h"

ATwoMinPlayerCharacter::ATwoMinPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = CameraBoomLength;
	CameraBoom->SocketOffset = SocketOffSet;
	CameraBoom->CameraLagSpeed = CameraLagSpeed;
	CameraBoom->CameraRotationLagSpeed = CameraRotationLagSpeed;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->FieldOfView = FieldOfView;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->RotationRate = CharacterRotationRate;

	PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>("PlayerCombatComponent");
	
	bIsRun = false;
	CharacterType = ECharacterType::Player;
}

UBaseCombatComponent* ATwoMinPlayerCharacter::GetCombatComponent() const
{
	return PlayerCombatComponent;
}

ATwoMinPlayerController* ATwoMinPlayerCharacter::GetPlayerController() const
{
	return Cast<ATwoMinPlayerController>(GetController());
}

ATwoMinEnemyCharacter* ATwoMinPlayerCharacter::GetCurrentAutoTarget() const
{
	return GetCombatComponent()->GetAutoTargetingComponent()->GetCurrentTargetingActor();
}

void ATwoMinPlayerCharacter::CancelInputToggle()
{
	bIsWalk = false;
	bIsRun = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

FVector ATwoMinPlayerCharacter::GetInputDirection() const
{
	return GetLastMovementInputVector();
}

void ATwoMinPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
		}
	}
}

void ATwoMinPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	
	UCharacterInputComponent* CharacterInputComponent = CastChecked<UCharacterInputComponent>(PlayerInputComponent);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_Move,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_Move,
	ETriggerEvent::Completed, this, &ThisClass::Stoped);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_Look,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_SwitchTarget,
		ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTrigger);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_SwitchTarget,
	ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetComplete);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_Toggle_Run,
		ETriggerEvent::Started, this, &ThisClass::Input_ToggleRun);

	CharacterInputComponent->BindAbilityInputAction(InputConfigDataAsset, this,
			&ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void ATwoMinPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.IsNearlyZero() == false)
	{
		if (MovePossibleCancelAbilityTags.IsEmpty() == false && MovePossibleCancelAbilityTags.Num() > 0)
		{
			FGameplayTagContainer CancelTagContainer;
			for (FGameplayTag& CancelTag : MovePossibleCancelAbilityTags)
			{
				CancelTagContainer.AddTag(CancelTag);
			}

			AbilitySystemComponent->CancelAbilities(&CancelTagContainer);	
		}
	}
	
	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}

	bIsWalk = true;
}

void ATwoMinPlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	
	if (LookAxisVector.X != 0.f)
	{
		float AxisVectorX = LookAxisVector.X * (IsUsingGamepad() ? GamepadSensitivityX : MouseSensitivityX);
		
		AddControllerYawInput(AxisVectorX);
	}

	if (LookAxisVector.Y != 0.f)
	{
		float AxisVectorY = bInvertPitch ? -LookAxisVector.Y : LookAxisVector.Y;
		AxisVectorY *= (IsUsingGamepad() ? GamepadSensitivityY : MouseSensitivityY);
		
		AddControllerPitchInput(AxisVectorY);
	}
}

void ATwoMinPlayerCharacter::Input_SwitchTargetTrigger(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void ATwoMinPlayerCharacter::Input_SwitchTargetComplete(const FInputActionValue& InputActionValue)
{
	FGameplayEventData EventData;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X > 0.f ?
		TwoMinGameplayTag::Player_Event_SwitchTarget_Right :
		TwoMinGameplayTag::Player_Event_SwitchTarget_Left,
		EventData
	);
}

void ATwoMinPlayerCharacter::Stoped(const FInputActionValue& InputActionValue)
{
	bIsWalk = false;
	bIsRun = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void ATwoMinPlayerCharacter::Input_ToggleRun(const FInputActionValue& InputActionValue)
{
	if (bIsWalk == false)
	{
		return;
	}
	
	bIsRun = !bIsRun;
	GetCharacterMovement()->MaxWalkSpeed = bIsRun ? MaxRunSpeed : MaxWalkSpeed;
}

bool ATwoMinPlayerCharacter::IsUsingGamepad() const
{
	const UInputDeviceSubsystem* InputDeviceSubsystem =
		GetGameInstance()->GetEngine()->GetEngineSubsystem<UInputDeviceSubsystem>();
	
	if (InputDeviceSubsystem)
	{
		FHardwareDeviceIdentifier MostRecentDevice =
			InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(GetPlatformUserId());
		if (MostRecentDevice.PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad)
		{
			return true; // 게임패드 사용 중
		}
	}
	
	return false;
}

void ATwoMinPlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	AbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void ATwoMinPlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	AbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
