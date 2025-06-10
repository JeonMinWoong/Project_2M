// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TwoMinPlayerCharacter.h"

#include "ToMinGameplayTag.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/CharacterInputComponent.h"

ATwoMinPlayerCharacter::ATwoMinPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
	
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

	bIsRun = false;
}

void ATwoMinPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(ToMinAbilitySystemComponent);
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

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, ToMinGameplayTag::InputTag_Move,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, ToMinGameplayTag::InputTag_Move,
	ETriggerEvent::Completed, this, &ThisClass::Stoped);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, ToMinGameplayTag::InputTag_Look,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, ToMinGameplayTag::InputTag_Toggle_Run,
		ETriggerEvent::Started, this, &ThisClass::Input_ToggleRun);
}

void ATwoMinPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	
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
