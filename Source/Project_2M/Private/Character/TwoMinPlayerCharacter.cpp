// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TwoMinPlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TwoMinGameplayTag.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
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
#include "GameModes/TwoMinBaseGameMode.h"
#include "Input/CharacterInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Player/TwoMinWidgetPlayer.h"

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
	UDataAsset_StartUpDataBase* LoadData = CharacterStartUpData.LoadSynchronous();
	if (LoadData)
	{
		LoadData->StartUpDataLevelUp(AbilitySystemComponent, NewLevel);	
	}
	
	ATwoMinBaseGameMode* GM =Cast<ATwoMinBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return;
	if (GM->IsLoadData()) return;
	
	if (LevelUpEffect)
	{
		if (LevelUpNiagaraComp && LevelUpTimerHandle.IsValid())
		{
			LevelUpNiagaraComp->DestroyComponent();
			LevelUpNiagaraComp = nullptr;
			
			GetWorld()->GetTimerManager().ClearTimer(LevelUpTimerHandle);	
		}
		
		LevelUpNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			LevelUpEffect,
			GetMesh(),
			TwoMinConstant::Player_Center,       
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);
		
		GetWorld()->GetTimerManager().SetTimer(LevelUpTimerHandle, [this]()
		{
			if (LevelUpNiagaraComp)
			{
				LevelUpNiagaraComp->Deactivate();
				LevelUpNiagaraComp = nullptr;
			}
		}, 1.f, false);
	}
	
	if (LevelUpSound)
	{
		UTwoMinFunctionLibrary::PlaySound2D(this, LevelUpSound);
	}
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
	
	if (HUDOverlayClass)
	{
		HUDOverlay = CreateWidget<UUserWidget>(GetPlayerController(), HUDOverlayClass);
		if (HUDOverlay)
		{
			HUDOverlay->AddToViewport();
		}
	}
}

void ATwoMinPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATwoMinPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckSpecialAttack(DeltaTime);
	CheckRunState(DeltaTime);
}

void ATwoMinPlayerCharacter::CheckSpecialAttack(float DeltaTime)
{
	if (bIsSpecialAttackCheck == false)
	{
		return;
	}
	
	CurDelay += DeltaTime;
	if (CurDelay >= InputDelay)
	{
		bIsSpecialAttackCheck = false;
		CurDelay = 0.f;
	}
}

void ATwoMinPlayerCharacter::CheckRunState(float DeltaTime)
{
	if (bIsRunning == false)
	{
		InputRunningTime = 0;
		MoveStop(FInputActionValue());
		return;
	}
	
	InputRunningTime += DeltaTime;
	if (InputRunningTime < MaxInputRunningTime) return;
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_Running)) return;
	
	UTwoMinFunctionLibrary::AddGameplayTagToActor(this, TwoMinGameplayTag::Player_State_Running);
	GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;
}

void ATwoMinPlayerCharacter::AfterDeathProcess()
{
	Super::AfterDeathProcess();
	
	DefeatStateProcess();
}

void ATwoMinPlayerCharacter::DefeatStateProcess()
{
	ATwoMinBaseGameMode* GM = Cast<ATwoMinBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return;
	
	GM->ShowDefeatStageUI();
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
	ETriggerEvent::Completed, this, &ThisClass::MoveStop);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_Look,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_SwitchTarget,
		ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTrigger);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_SwitchTarget,
	ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetComplete);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_MustBeHold_Run,
		ETriggerEvent::Triggered, this, &ThisClass::Input_OnRun);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_MustBeHold_Run,
	ETriggerEvent::Completed, this, &ThisClass::MoveStop);

	CharacterInputComponent->BindAbilityInputAction(InputConfigDataAsset, this,
			&ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);

	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_ObjectTargeting,
	ETriggerEvent::Started, this, &ThisClass::Input_ObjectiveTargetingTrigger);
	
	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_OpenInventory,
		ETriggerEvent::Started, this, &ThisClass::Input_OpenInventory);
	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_UseItem, 
		ETriggerEvent::Started, this, &ThisClass::Input_UseItemTrigger);
	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_LeftQuickSlot, 
		ETriggerEvent::Started, this, &ThisClass::Input_LeftQuickSlotTrigger);
	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_RightQuickSlot, 
		ETriggerEvent::Started, this, &ThisClass::Input_RightQuickSlotItemTrigger);
	
	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_SpecialAttack_Check, 
		ETriggerEvent::Started, this, &ThisClass::Input_SpecialAttack_Check_Trigger);
	CharacterInputComponent->BindNativeInputAction(InputConfigDataAsset, TwoMinGameplayTag::InputTag_EndGame,
		ETriggerEvent::Started, this, &ThisClass::Input_EndGameTrigger);
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

void ATwoMinPlayerCharacter::MoveStop(const FInputActionValue& InputActionValue)
{
	bIsWalk = false;
	bIsRunning = false;
	UTwoMinFunctionLibrary::RemoveGameplayTagToActor(this, TwoMinGameplayTag::Player_State_Running);
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void ATwoMinPlayerCharacter::Input_OnRun(const FInputActionValue& InputActionValue)
{
	for (auto GameplayTag : RunIgnoreTagContainer)
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(this, GameplayTag))
		{
			MoveStop(FInputActionValue());
			return;
		}
	}
	
	float CurStaminaValue =
		GetAbilitySystemComponent()->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentStaminaAttribute());
	if (CurStaminaValue <= 0.f) 
	{
		MoveStop(FInputActionValue());
		FGameplayEventData EventData;
		EventData.Instigator = this;
		
		UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
			this, 
			TwoMinGameplayTag::Shared_Event_Exhausted,
			EventData
		);
		
		return;
	}
	
	bIsRunning = true;
}

bool ATwoMinPlayerCharacter::IsUsingGamepad() const
{
	return UTwoMinFunctionLibrary::IsUsingGamePad(GetWorld(), GetPlatformUserId());
}

void ATwoMinPlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitDowning)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitThrowing)
			|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_OpenInventory))
	{
		return;
	}
	
	if (InInputTag == TwoMinGameplayTag::InputTag_LightAttack_OneHand ||
		InInputTag == TwoMinGameplayTag::InputTag_HeavyAttack_OneHand)
	{
		if (bIsSpecialAttackCheck)
		{
			return;
		}
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

void ATwoMinPlayerCharacter::Input_ObjectiveTargetingTrigger(const FInputActionValue& InputActionValue)
{
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_Possible_ItemPickUp))
	{
		Input_PickUpTrigger(InputActionValue);
		return;
	}
	
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_Possible_Interaction))
	{
		Input_InteractTrigger(InputActionValue);
		return;
	}
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

void ATwoMinPlayerCharacter::Input_InteractTrigger(const FInputActionValue& InputActionValue)
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
		TwoMinGameplayTag::Player_Event_Interaction,
		EventData
	);
}

void ATwoMinPlayerCharacter::Input_OpenInventory(const FInputActionValue& InputActionValue)
{
	if (bIsEndGameTrigger || PlayerUIComponent->IsMapSelectWidgetOpen() || PlayerUIComponent->IsStoreWidgetOpen()) return;
	
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

void ATwoMinPlayerCharacter::PlayCameraShakeOnHit(ECameraShakeType CameraShakeType)
{
	if (CameraShakeMap.Contains(CameraShakeType) == false) return;
	
	if (CachedCameraShake)
	{
		if (CachedCameraShake->IsActive())
		{
			if (CameraShakeType < CachedCameraShakeType) return;

			CachedCameraShake->StopShake();
			CachedCameraShake = nullptr;	
		}
	}
	
	CachedCameraShakeType = CameraShakeType;
	CachedCameraShake = GetPlayerController()->PlayerCameraManager->StartCameraShake(CameraShakeMap[CameraShakeType]);
}

void ATwoMinPlayerCharacter::OnIgnoreInputProcess(bool bIsIgnore)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	if (bIsIgnoreInput == bIsIgnore) return;
	
	bIsIgnoreInput = bIsIgnore;
	PC->SetIgnoreMoveInput(bIsIgnore);
	PC->SetIgnoreLookInput(bIsIgnore);
	PC->FlushPressedKeys();
	
	if (bIsIgnore)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
		PC->SetInputMode(InputMode);	
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}

bool ATwoMinPlayerCharacter::IsPossibleUseItem()
{
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitDowning)
		|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Shared_State_HitThrowing)
			|| UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_UseItem))
	{
		return false;
	}
	
	for (auto GameplayTag : IgnoreTagContainer)
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(this, GameplayTag))
		{
			return false;
		}
	}
	
	return true;
}

void ATwoMinPlayerCharacter::Input_UseItemTrigger(const FInputActionValue& InputActionValue)
{
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_OpenInventory)) return;
	
	UTwoMinWidgetPlayer* WidgetPlayer = Cast<UTwoMinWidgetPlayer>(HUDOverlay);
	if (!WidgetPlayer) return;
	if (IsPossibleUseItem() == false) return;
	
	int32 SlotIndex = WidgetPlayer->GetWindowQuickSlot()->GetCurrentSlotIndex();
	FItemInstance ItemInstance = InventoryComponent->GetQuickSlotItemInstance(SlotIndex);
	if (ItemInstance.ItemID == 0) return;
	
	bool bIsRemoved = false;
	InventoryComponent->UseItem(ItemInstance.ItemID, bIsRemoved); // Test Health Potion
	
	if (ItemInstance.bIsRegister && bIsRemoved)
	{
		PlayerUIComponent->OnSetWindowQuickSlot.Broadcast(ItemInstance, ItemInstance.RegisterCount, false);
	}
}

void ATwoMinPlayerCharacter::Input_LeftQuickSlotTrigger(const FInputActionValue& InputActionValue)
{
	UTwoMinWidgetPlayer* WidgetPlayer = Cast<UTwoMinWidgetPlayer>(HUDOverlay);
	if (!WidgetPlayer) return;

	int32 CurrentSlotIndex = WidgetPlayer->GetWindowQuickSlot()->GetCurrentSlotIndex();
	int32 NewSlotIndex;
	if (CurrentSlotIndex <= WidgetPlayer->GetWindowQuickSlot()->GetMinSlotIndex())
	{
		NewSlotIndex = WidgetPlayer->GetWindowQuickSlot()->GetMaxSlotIndex();
	}
	else
	{
		NewSlotIndex = CurrentSlotIndex - 1;
	}
	
	WidgetPlayer->GetWindowQuickSlot()->SetCurrentSlotIndex(NewSlotIndex);
}

void ATwoMinPlayerCharacter::Input_RightQuickSlotItemTrigger(const FInputActionValue& InputActionValue)
{
	UTwoMinWidgetPlayer* WidgetPlayer = Cast<UTwoMinWidgetPlayer>(HUDOverlay);
	if (!WidgetPlayer) return;

	int32 CurrentSlotIndex = WidgetPlayer->GetWindowQuickSlot()->GetCurrentSlotIndex();
	int32 NewSlotIndex;
	if (CurrentSlotIndex >= WidgetPlayer->GetWindowQuickSlot()->GetMaxSlotIndex())
	{
		NewSlotIndex = WidgetPlayer->GetWindowQuickSlot()->GetMinSlotIndex();
	}
	else
	{
		NewSlotIndex = CurrentSlotIndex + 1;
	}
	
	WidgetPlayer->GetWindowQuickSlot()->SetCurrentSlotIndex(NewSlotIndex);
}

void ATwoMinPlayerCharacter::Input_SpecialAttack_Check_Trigger(const FInputActionValue& InputActionValue)
{
	FGameplayTag InputTag = TwoMinGameplayTag::InputTag_SpecialAttack_OneHand;
	FName FightCostName = "Player.SpecialAttack.OneHand";
	if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_FullFight))
	{
		FightCostName = "Player.Anger.InRush";
		InputTag = TwoMinGameplayTag::InputTag_AngerMode_Inrush;
	}
	else if (UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_AngerMode))
	{
		FightCostName = "Player.SpecialAttack.Anger.OneHand";
		InputTag = TwoMinGameplayTag::InputTag_AngerMode_SpecialAttack_OneHand;
	}
	
	float CurFightValue =
		GetAbilitySystemComponent()->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentFightAttribute());
	const FRealCurve* Curve = FightCurveTable->FindCurve(FightCostName, "1");
	const float FightCost = Curve->Eval(1);
	
	if (CurFightValue < FightCost)
	{
		return;
	}
	
	bIsSpecialAttackCheck = true;
	Input_AbilityInputPressed(InputTag);
}

void ATwoMinPlayerCharacter::Input_EndGameTrigger(const FInputActionValue& InputActionValue)
{
	if (bIsOpenInventory || PlayerUIComponent->IsMapSelectWidgetOpen() || PlayerUIComponent->IsStoreWidgetOpen()) return;
	
	OpenEndGameProcess();
}

void ATwoMinPlayerCharacter::OpenEndGameProcess()
{
	bIsEndGameTrigger = !bIsEndGameTrigger;
	PlayerUIComponent->OpenEndGameWidget(this, bIsEndGameTrigger);
}

bool ATwoMinPlayerCharacter::GetIsRunning()
{
	return UTwoMinFunctionLibrary::HasGameplayTag(this, TwoMinGameplayTag::Player_State_Running);
}
