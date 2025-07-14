// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_LockOn_Player.h"

#include "EnhancedInputSubsystems.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Ability/PlayerTask/TwoMinAT_LockOn_Player.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Camera/CameraComponent.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Components/SizeBox.h"
#include "Controller/TwoMinPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/TwoMinWidgetBase.h"


void UTwoMinGA_LockOn_Player::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	DebugTwoMin::Print(TEXT("Lock On Ability Activated"), FColor::Green, 1);
	LockOnTarget = FindLockOnTarget();
	if (!LockOnTarget)
	{
		DebugTwoMin::Print(TEXT("Lock On Target Null"), FColor::Red, 2);
		CustomCancelAbility();
		return;
	}

	FString TargetName = FString::Printf(TEXT("Lock On Target Name: %s"), *LockOnTarget->GetActorNameOrLabel());
	DebugTwoMin::Print(TargetName, FColor::Green, 2);

	DrawLockOnWidget();
	SetTargetLockOnWidgetPosition();
	ChangeMappingContext();
	LockCharacterMovement();
	StartLockOnTickTask();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_LockOn_Player::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	DebugTwoMin::Print(TEXT("Lock On Ability End"), FColor::Blue, 3);
	EndLockOnTarget();
	ResetMappingContext();
	ResetCharacterMovement();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AActor* UTwoMinGA_LockOn_Player::FindLockOnTarget()
{
	LockOnTarget = nullptr;
	CheckSphereOverTargetGroup();

	if (LockOnTargetGroup.IsEmpty())
	{
		return nullptr;
	}
	
	float CurDistance = FLT_MAX;
	for (AActor* Target : LockOnTargetGroup)
	{
		if (IsLockOnScreenToDistance(Target, CurDistance) == false)
		{
			continue;
		}

		LockOnTarget = Target;
	}
	
	return LockOnTarget;
}

void UTwoMinGA_LockOn_Player::CheckSphereOverTargetGroup()
{
	LockOnTargetGroup.Empty();

	TArray<AActor*> EmptyLockOnTargetGroup;
	const FVector Center = GetAvatarActorFromActorInfo()->GetActorLocation();
	const TArray<AActor*> IgnoreActors;

	if (bIsDebugLockOnRange)
	{
		DrawDebugSphere(GetWorld(), Center, LockOnDistance, 32, FColor::Yellow, false,
			2.0f, 0, 2.0f);	
	}
	
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Center,
		LockOnDistance,
		ObjectTypes,
		ATwoMinEnemyCharacter::StaticClass(),
		IgnoreActors,
		EmptyLockOnTargetGroup
	);

	if (EmptyLockOnTargetGroup.IsEmpty())
	{
		return;
	}
	
	for (AActor* Target : EmptyLockOnTargetGroup)
	{
		if (IsLockOnCondition(Target) == false)
		{
			continue;
		}

		LockOnTargetGroup.AddUnique(Target);
	}
}


bool UTwoMinGA_LockOn_Player::IsLockOnCondition(const AActor* Target) const
{
	if (IsValid(Target) == false)
	{
		return false;
	}

	// 1. 거리 만족
	FVector PlayerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	if (IsLockOnDistance(PlayerLocation, TargetLocation) == false)
	{
		return false;
	}

	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter)
	{
		return false;
	}
	
	FVector CameraLocation = PlayerCharacter->GetCamera()->GetComponentLocation();
	if (IsLockOnLineTraceHit(CameraLocation, TargetLocation) == false)
	{
		return false;
	}
	
	// 2. 각도 만족
	FVector CameraForward = PlayerCharacter->GetCamera()->GetForwardVector();
	CameraForward.Z = 0.f;
	
	FVector ToTarget = TargetLocation - PlayerLocation;
	ToTarget.Z = 0.f;

	CameraForward.Normalize();
	ToTarget.Normalize();
	
	float AngleDeg = UKismetMathLibrary::DegAcos(FVector::DotProduct(CameraForward, ToTarget));
	if (AngleDeg > LockOnAngle)
	{
		return false;
	}
	
	return true;
}

bool UTwoMinGA_LockOn_Player::IsLockOnDistance(const FVector& PlayerLocation, const FVector& TargetLocation) const
{
	const float Distance = FVector::Dist(PlayerLocation, TargetLocation);
	return Distance <= LockOnDistance;
}

bool UTwoMinGA_LockOn_Player::IsLockOnLineTraceHit(FVector CameraLocation, FVector TargetLocation) const
{
	FHitResult HitResult;
	const TArray<AActor*> IgnoreActors;
	
	bool IsHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		CameraLocation,
		TargetLocation,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (IsHit && HitResult.bBlockingHit)
	{
		ATwoMinEnemyCharacter* Enemy = Cast<ATwoMinEnemyCharacter>(HitResult.GetActor());
		if (!Enemy)
		{
			return false;
		}
	}
	
	return true;
}

// 3. 화면 중앙과의 거리 순
bool UTwoMinGA_LockOn_Player::IsLockOnScreenToDistance(const AActor* Target, float& ClosestDistance) const
{
	FVector2D ScreenCenter;
	FVector2D ScreenPos;
	if (IsInScreenPos(Target, ScreenCenter, ScreenPos) == false)
	{
		return false;
	}
	
	float DistToCenter = FVector2D::Distance(ScreenPos, ScreenCenter);
	if (DistToCenter >= ClosestDistance)
	{
		return false;
	}

	ClosestDistance = DistToCenter;
	return true;
}

bool UTwoMinGA_LockOn_Player::IsInScreenPos(const AActor* Target, FVector2D& ScreenCenter, FVector2D& ScreenPos) const
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter)
	{
		return false;
	}
	
	int32 ScreenX, ScreenY;
	ATwoMinPlayerController* PlayerController = PlayerCharacter->GetPlayerController();
	PlayerController->GetViewportSize(ScreenX, ScreenY);
	ScreenCenter = FVector2D(ScreenX / 2.f, ScreenY / 2.f);

	if (PlayerController->ProjectWorldLocationToScreen(Target->GetActorLocation(), ScreenPos) == false)
	{
		return false;
	}
	
	return true;
}

void UTwoMinGA_LockOn_Player::DrawLockOnWidget()
{
	if (!LockOnTargetWidgetClass)
	{
		return;
	}

	ATwoMinPlayerController* PlayerController = Cast<ATwoMinPlayerController>(CurrentActorInfo->PlayerController);
	LockOnTargetWidget = CreateWidget<UTwoMinWidgetBase>(PlayerController, LockOnTargetWidgetClass);
	if (!LockOnTargetWidget)
	{
		return;
	}

	LockOnTargetWidget->AddToViewport();
}

void UTwoMinGA_LockOn_Player::SetTargetLockOnWidgetPosition()
{
	if (!LockOnTargetWidget || !LockOnTarget)
	{
		CustomCancelAbility();
		return;
	}

	if (IsLockOnDistance(GetAvatarActorFromActorInfo()->GetActorLocation(),
		LockOnTarget->GetActorLocation()) == false)
	{
		CustomCancelAbility();
		return;
	}

	ATwoMinPlayerController* PlayerController = Cast<ATwoMinPlayerController>(CurrentActorInfo->PlayerController);
	
	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		PlayerController,
		LockOnTarget->GetActorLocation(),
		ScreenPosition,
		true
	);

	if (LockOnTargetWidgetSize == FVector2D::ZeroVector)
	{
		LockOnTargetWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (const USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))		
				{
					LockOnTargetWidgetSize.X = FoundSizeBox->GetWidthOverride();
					LockOnTargetWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);	
	}

	ScreenPosition -= (LockOnTargetWidgetSize / 2.f);
	
	LockOnTargetWidget->SetPositionInViewport(ScreenPosition, false);
}

void UTwoMinGA_LockOn_Player::LockCharacterMovement()
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter)
	{
		return;
	}

	 PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UTwoMinGA_LockOn_Player::StartLockOnTickTask()
{
	LockOnTickTask = UTwoMinAT_LockOn_Player::CreateTickTask(this);
	if (!LockOnTickTask)
	{
		CustomCancelAbility();
		return;
	}

	LockOnTickTask->OnLockOnTick.AddUniqueDynamic(this, &UTwoMinGA_LockOn_Player::UpdateLockOnTarget);
	LockOnTickTask->ReadyForActivation();

	LockOnSwitchTargetEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this,
	FGameplayTag::RequestGameplayTag("Player.Event.SwitchTarget"),
	nullptr,
	false,
	false
	);

	if (!LockOnSwitchTargetEvent)
	{
		CustomCancelAbility();
		return;
	}

	LockOnSwitchTargetEvent->EventReceived.AddUniqueDynamic(this, &UTwoMinGA_LockOn_Player::OnSwitchTarget);
	LockOnSwitchTargetEvent->ReadyForActivation();
}

void UTwoMinGA_LockOn_Player::ChangeMappingContext()
{
	const ULocalPlayer* LocalPlayer = GetActorInfo().PlayerController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(LockOnInputMappingContext, 2);
}

void UTwoMinGA_LockOn_Player::UpdateLockOnTarget(float DeltaTime)
{
	if (!LockOnTarget || !CurrentActorInfo || !LockOnTickTask)
	{
		CustomCancelAbility();
		return;
	}

	if (IsLockOnDistance(GetAvatarActorFromActorInfo()->GetActorLocation(),
		LockOnTarget->GetActorLocation()) == false)
	{
		CustomCancelAbility();
		return;
	}

	SetTargetLockOnWidgetPosition();

	
	
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
	GetAvatarActorFromActorInfo()->GetActorLocation(),
	LockOnTarget->GetActorLocation());

	LookAtRot -= FRotator(LockOnCameraOffsetDistance, 0, 0.f);
		
	const FRotator CurrentControlRot = GetActorInfo().PlayerController->GetControlRotation();
	const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, LockOnCameraRotationSpeed);

	// 1
	GetActorInfo().PlayerController->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));

	if (UTwoMinFunctionLibrary::HasGameplayTag(GetAvatarActorFromActorInfo(), TwoMinGameplayTag::Player_State_Rolling))
	{
		DebugTwoMin::Print(TEXT("Roll !!!!!!!!"), FColor::Red, 4);
		return;
	}
	
	const FRotator CharacterRot = GetAvatarActorFromActorInfo()->GetActorRotation();
	FRotator NewCharacterRot = FMath::RInterpTo(CharacterRot,
		FRotator(0.f, TargetRot.Yaw, 0.f), DeltaTime, LockOnCharacterRotationSpeed);

	GetAvatarActorFromActorInfo()->SetActorRotation(FRotator(0.f, NewCharacterRot.Yaw, 0.f));
}

void UTwoMinGA_LockOn_Player::OnSwitchTarget(FGameplayEventData InputEventData)
{
	if (CurrentSwitchTime > GetWorld()->GetTimeSeconds())
	{
		return;
	}

	CurrentSwitchTime = GetWorld()->GetTimeSeconds() + LockOnSwitchTime;
	
	FString Value = FString::Printf(TEXT("Switch Target %s"), *InputEventData.EventTag.ToString());
	DebugTwoMin::Print(Value, FColor::Yellow, 4);
	
	CheckSphereOverTargetGroup();

	if (LockOnTargetGroup.IsEmpty())
	{
		return;
	}

	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	OnSplitLeftRightTargetGroup(ActorsOnLeft, ActorsOnRight);

	float ClosestDistance = FLT_MAX;
	if (InputEventData.EventTag == TwoMinGameplayTag::Player_Event_SwitchTarget_Left)
	{
		for (AActor* Target : ActorsOnLeft)
		{
			if (IsLockOnScreenToDistance(Target, ClosestDistance) == false)
			{
				continue;
			}

			NewTargetToLock = Target;
		}
	}
	else
	{
		for (AActor* Target : ActorsOnRight)
		{
			if (IsLockOnScreenToDistance(Target, ClosestDistance) == false)
			{
				continue;
			}

			NewTargetToLock = Target;
		}
	}

	if (NewTargetToLock)
	{
		LockOnTarget = NewTargetToLock;
	}
}

void UTwoMinGA_LockOn_Player::OnSplitLeftRightTargetGroup(TArray<AActor*>& ActorsOnLeft, TArray<AActor*>& ActorsOnRight)
{
	if (!LockOnTarget || LockOnTargetGroup.IsEmpty())
	{
		CustomCancelAbility();
		return;	
	}

	ATwoMinPlayerController* PlayerController = Cast<ATwoMinPlayerController>(CurrentActorInfo->PlayerController);
	if (!PlayerController)
	{
		return;
	}

	for (AActor* Target : LockOnTargetGroup)
	{
		if (!Target || Target == LockOnTarget)
		{
			continue;	
		}

		FVector2D ScreenCenter;
		FVector2D ScreenPos;
		if (IsInScreenPos(Target, ScreenCenter, ScreenPos) == false)
		{
			continue;
		}

		if (ScreenPos.X < ScreenCenter.X)
		{
			ActorsOnLeft.AddUnique(Target);
		}
		else
		{
			ActorsOnRight.AddUnique(Target);
		}
	}
}

void UTwoMinGA_LockOn_Player::EndLockOnTarget()
{
	LockOnTargetGroup.Empty();

	LockOnTarget = nullptr;
	
	if (LockOnTargetWidget)
	{
		LockOnTargetWidget->RemoveFromParent();
	}

	LockOnTargetWidget = nullptr;
	LockOnTargetWidgetSize = FVector2D::ZeroVector;

	if (LockOnTickTask)
	{
		LockOnTickTask->EndTask();
		LockOnTickTask = nullptr;	
	}

	bIsCharacterRotationLock = false;
	CurrentSwitchTime = 0.f;
}

void UTwoMinGA_LockOn_Player::ResetMappingContext()
{
	ATwoMinPlayerController* PlayerController = Cast<ATwoMinPlayerController>(GetActorInfo().PlayerController);
	if (!PlayerController)
	{
		return;	
	}
	
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->RemoveMappingContext(LockOnInputMappingContext);
}

void UTwoMinGA_LockOn_Player::ResetCharacterMovement()
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter)
	{
		return;
	}

	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}
