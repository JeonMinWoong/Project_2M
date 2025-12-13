// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TwoMinPlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TwoMinGameplayTag.h"
#include "EnhancedInputSubsystems.h"
#include "TwoMinFunctionLibrary.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Compnents/AutoTargetingComponent.h"
#include "Compnents/InventoryComponent.h"
#include "Compnents/Combat/PlayerCombatComponent.h"
#include "Compnents/UI/PlayerUIComponent.h"
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
	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>("PlayerUIComponent");
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	CharacterType = ECharacterType::Player;
}

UBaseCombatComponent* ATwoMinPlayerCharacter::GetCombatComponent() const
{
	return PlayerCombatComponent;
}

UBaseUIComponent* ATwoMinPlayerCharacter::GetBaseUIComponent() const
{
	return PlayerUIComponent;
}

UPlayerUIComponent* ATwoMinPlayerCharacter::GetPlayerUIComponent() const
{
	return PlayerUIComponent;
}

ATwoMinPlayerController* ATwoMinPlayerCharacter::GetPlayerController() const
{
	return Cast<ATwoMinPlayerController>(GetController());
}

ATwoMinEnemyCharacter* ATwoMinPlayerCharacter::GetCurrentAutoTarget() const
{
	return GetCombatComponent()->GetAutoTargetingComponent()->GetCurrentTargetingActor();
}

UInventoryComponent* ATwoMinPlayerCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

void ATwoMinPlayerCharacter::CancelInputToggle()
{
	bIsWalk = false;
	UTwoMinFunctionLibrary::RemoveGameplayTagToActor(this, TwoMinGameplayTag::Player_State_Running);
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

FVector ATwoMinPlayerCharacter::GetInputDirection() const
{
	return GetLastMovementInputVector();
}

void ATwoMinPlayerCharacter::PlayerLevelUp(int32 NewLevel)
{
	CharacterStartUpData->StartUpDataLevelUp(AbilitySystemComponent, NewLevel);
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

void ATwoMinPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayClass)
	{
		HUDOverlay = CreateWidget<UUserWidget>(GetPlayerController(), HUDOverlayClass);
		if (HUDOverlay)
		{
			HUDOverlay->AddToViewport();
		}
	}
}

void ATwoMinPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 1);
	
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

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_MustBeHold_Run,
		ETriggerEvent::Triggered, this, &ThisClass::Input_OnRun);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_MustBeHold_Run,
	ETriggerEvent::Completed, this, &ThisClass::Stoped);

	CharacterInputComponent->BindAbilityInputAction(InputConfigDataAsset, this,
			&ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_ItemPickUp,
	ETriggerEvent::Started, this, &ThisClass::Input_PickUpTrigger);
	
	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_OpenInventory,
		ETriggerEvent::Started, this, &ThisClass::Input_OpenInventory);
	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_UseItem, 
		ETriggerEvent::Started, this, &ThisClass::Input_UseItemTrigger);
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
				if (UTwoMinGameplayAbility* Ability = AbilitySystemComponent->GetPlayingAbilityTag(CancelTag))
				{
					Ability->CustomCancelAbility();
				}
			}
			
			AbilitySystemComponent->CancelAbilities(&CancelTagContainer);	
		}
	}

	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_Exhausted)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitDowning)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitThrowing)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_Death)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_OpenInventory))
	{
		return;
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

	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_Death)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_Execution_Caster) 
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_OpenInventory))
	{
		return;
	}
	
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
	UTwoMinFunctionLibrary::RemoveGameplayTagToActor(this, TwoMinGameplayTag::Player_State_Running);
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void ATwoMinPlayerCharacter::Input_OnRun(const FInputActionValue& InputActionValue)
{
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_Attacking))
	{
		return;
	}
	
	UTwoMinFunctionLibrary::AddGameplayTagToActor(this, TwoMinGameplayTag::Player_State_Running);
	GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;
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
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitDowning)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitThrowing)
			|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_OpenInventory))
	{
		return;
	}
	
	AbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void ATwoMinPlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitDowning)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitThrowing)
			|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_OpenInventory))
	{
		return;
	}
	
	AbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void ATwoMinPlayerCharacter::Input_PickUpTrigger(const FInputActionValue& InputActionValue)
{
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitDowning)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitThrowing)
			|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_OpenInventory))
	{
		return;
	}
	
	for (auto GameplayTag : IgnoreTagContainer)
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(this, GameplayTag))
		{
			return;
		}
	}
	
	FGameplayEventData EventData;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		TwoMinGameplayTag::Player_Event_ItemPickUp,
		EventData
	);
}

void ATwoMinPlayerCharacter::Input_OpenInventory(const FInputActionValue& InputActionValue)
{
	OpenInventoryProcess();
}

void ATwoMinPlayerCharacter::OpenInventoryProcess()
{
	bIsOpenInventory = !bIsOpenInventory;
	
	if (bIsOpenInventory)
	{
		UTwoMinFunctionLibrary::AddGameplayTagToActor(
			this,
			TwoMinGameplayTag::Player_State_OpenInventory
		);
	}
	else
	{
		UTwoMinFunctionLibrary::RemoveGameplayTagToActor(
			this,
			TwoMinGameplayTag::Player_State_OpenInventory
		);
	}
	
	InventoryComponent->OpenInventory(bIsOpenInventory);
}

void ATwoMinPlayerCharacter::Input_UseItemTrigger(const FInputActionValue& InputActionValue)
{
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitDowning)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitThrowing)
			|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_OpenInventory))
	{
		return;
	}
	
	for (auto GameplayTag : IgnoreTagContainer)
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(this, GameplayTag))
		{
			return;
		}
	}
	
	// todo : 퀵 슬롯 작업.
	InventoryComponent->UseItem(20002); // Test Health Potion
}

bool ATwoMinPlayerCharacter::GetIsRunning()
{
	return UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_Running);
}
