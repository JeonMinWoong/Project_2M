// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_AimingBase.h"

#include "MotionWarpingComponent.h"
#include "AbilitySystem/Ability/Task/TwoMinAT_UpdateAiming.h"
#include "Blueprint/WidgetTree.h"
#include "Controller/TwoMinPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UTwoMinGA_AimingBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter)
	{
		CustomCancelAbility();
		return;
	}
	
	MyCharacter->SetIsAiming(true);
	
	DrawAimingWidget();
	PlayToAnimMontage(AimAnimMontage);
	StartAimingTick();
	LockCharacterMovement();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_AimingBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		MyCharacter->SetIsAiming(false);
	}
	
	HideAimingWidget();
	ResetCharacterMovement();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGA_AimingBase::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter)
	{
		CustomCancelAbility();
		return;
	}
	
	MyCharacter->SetIsAiming(false);
	MyCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Aiming_End"), AimAnimMontage);
	
	HideAimingWidget();
	ResetCharacterMovement();
}

void UTwoMinGA_AimingBase::DrawAimingWidget()
{
	if (!AimingWidgetClass) return;
	
	ATwoMinPlayerController* PlayerController = Cast<ATwoMinPlayerController>(CurrentActorInfo->PlayerController);
	if (!PlayerController) return;
	
	AimingWidget = CreateWidget<UTwoMinWidgetBase>(PlayerController, AimingWidgetClass);
	if (!AimingWidget) return;
	
	AimingWidget->AddToViewport();
	
	int32 ViewportSizeX, ViewportSizeY;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenCenter(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f);
	
	AimingWidget->SetPositionInViewport(ScreenCenter);
}

void UTwoMinGA_AimingBase::StartAimingTick()
{
	AimingTickTask = UTwoMinAT_UpdateAiming::CreateTickTask(this);
	if (!AimingTickTask)
	{
		CustomCancelAbility();
		return;
	}

	AimingTickTask->OnAimingTick.AddUniqueDynamic(this, &UTwoMinGA_AimingBase::UpdateAimingPoint);
	AimingTickTask->ReadyForActivation();
}

void UTwoMinGA_AimingBase::LockCharacterMovement()
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;

	PlayerCharacter->CancelInputToggle();
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	PlayerCharacter->GetMotionWarpingComponent()->RemoveWarpTarget("RotationDirection");
	PlayerCharacter->GetMotionWarpingComponent()->RemoveWarpTarget("TranslationDistance");
}

void UTwoMinGA_AimingBase::HideAimingWidget()
{
	if (AimingWidget)
	{
		AimingWidget->RemoveFromParent();
		AimingWidget = nullptr;
	}
	
	if (AimingTickTask)
	{
		AimingTickTask->EndTask();
		AimingTickTask = nullptr;	
	}
}

void UTwoMinGA_AimingBase::ResetCharacterMovement()
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter) return;

	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UTwoMinGA_AimingBase::UpdateAimingPoint(float DeltaTime)
{
	if (!AimingWidget || !AimingTickTask)
	{
		CustomCancelAbility();
		return;
	}
	
	ATwoMinPlayerController* PlayerController = Cast<ATwoMinPlayerController>(CurrentActorInfo->PlayerController);
	if (!PlayerController) return;
	
	FVector2D ViewportSize;
    GEngine->GameViewport->GetViewportSize(ViewportSize);
    FVector2D ScreenCenter(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	
    FVector WorldLoc, WorldDir;
	if (UGameplayStatics::DeprojectScreenToWorld(PlayerController, ScreenCenter, WorldLoc, WorldDir))
	{
		const FRotator CurrentControlRot = PlayerController->GetControlRotation();
		FRotator TargetRotation = WorldDir.Rotation();
	
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, TargetRotation, DeltaTime, 20.f);
		PlayerController->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		
		const FRotator CharacterRot = GetAvatarActorFromActorInfo()->GetActorRotation();
		FRotator NewCharacterRot = FMath::RInterpTo(CharacterRot,
			FRotator(0.f, TargetRotation.Yaw, 0.f), DeltaTime, 20.f);
		GetAvatarActorFromActorInfo()->SetActorRotation(FRotator(0.f, NewCharacterRot.Yaw, 0.f));
	}
}
