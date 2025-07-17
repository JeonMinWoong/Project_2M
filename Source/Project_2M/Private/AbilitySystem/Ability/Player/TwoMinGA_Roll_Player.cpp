// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/TwoMinGA_Roll_Player.h"

#include "MotionWarpingComponent.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/Player/TwoMinGA_LockOn_Player.h"
#include "Character/TwoMinPlayerCharacter.h"

UTwoMinGA_Roll_Player::UTwoMinGA_Roll_Player()
{
	bRetriggerInstancedAbility = UTwoMinGA_Roll_Player::bIsReTriggerSameAbility();
	AbilityInputType = ETwoAbilityInputType::ReTriggerable;
}

void UTwoMinGA_Roll_Player::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	//DebugTwoMin::Print(TEXT("Roll Ability Activated"), FColor::Green, 1);
	
	StartRoll(ActorInfo);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_Roll_Player::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	//DebugTwoMin::Print(TEXT("Roll Ability End"), FColor::Blue, 2);
	// EndRoll(ActorInfo);
	//
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UTwoMinGA_Roll_Player::bIsReTriggerSameAbility() const
{
	return true;
}

void UTwoMinGA_Roll_Player::StartRoll(const FGameplayAbilityActorInfo* ActorInfo)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(ActorInfo->OwnerActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->CancelInputToggle();
	}

	if (UTwoMinFunctionLibrary::HasGameplayTag(PlayerCharacter, TwoMinGameplayTag::Player_State_LockOn))
	{
		LockRoll(PlayerCharacter);
		return;
	}
	
	NormalRoll();
}

void UTwoMinGA_Roll_Player::NormalRoll()
{
	if (!RollMontages)
	{
		return;
	}

	PlayToAnimMontage(RollMontages);
}

void UTwoMinGA_Roll_Player::LockRoll(ATwoMinPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;
	
	if (LockRollMontages.IsEmpty()) return;

	FVector PlayerForward = PlayerCharacter->GetActorForwardVector().GetSafeNormal2D();
	FVector InputDirection = PlayerCharacter->GetInputDirection().GetSafeNormal2D();
	FRotator TargetRotator = PlayerCharacter->GetInputDirection().Rotation();

	
	float AngleRadians = FMath::Atan2(
		FVector::CrossProduct(PlayerForward, InputDirection).Z,
		FVector::DotProduct(PlayerForward, InputDirection)
	);

	int DirectionIndex = 0;

	float CorrectionAngle = 0.f;
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	if (AngleDegrees >= -45.f && AngleDegrees < 45.f)
	{
		DirectionIndex = 0; // 앞	
	}
	else if (AngleDegrees >= 45.f && AngleDegrees < 135.f)
	{
		DirectionIndex = 1; // 오른
		CorrectionAngle = -90.f;
	}
	else if (AngleDegrees >= -135.f && AngleDegrees < -45.f)
	{
		DirectionIndex = 2; // 왼
		CorrectionAngle = 90.f;
	}
	else if (AngleDegrees >= 135.f && AngleDegrees < 180.f)
	{
		DirectionIndex = 3; // 오른 뒤
		CorrectionAngle = 180.f;
	}
	else if (AngleDegrees > -180.f && AngleDegrees < -135.f)
	{
		DirectionIndex = 4; // 왼 뒤
		CorrectionAngle = 180.f;
	}

	TargetRotator += FRotator(0.f, CorrectionAngle, 0.f);
	PlayerCharacter->SetActorRotation(TargetRotator);
	
	DebugTwoMin::Print(FString::Printf(TEXT("%s, %s"),
		*FString::SanitizeFloat(AngleDegrees), *TargetRotator.ToString()), FColor::Yellow, 3);
	PlayToAnimMontage(LockRollMontages[DirectionIndex]);
}

void UTwoMinGA_Roll_Player::EndRoll(const FGameplayAbilityActorInfo* ActorInfo)
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(ActorInfo->OwnerActor);
	if (!PlayerCharacter)
	{
		return;
	}
	
	UMotionWarpingComponent* MotionWarpingComponent = PlayerCharacter->GetMotionWarpingComponent();
	if (!MotionWarpingComponent)
	{
		return;
	}
	
	UTwoMinGameplayAbility* Ability = PlayerCharacter->GetAbilitySystemComponent()->GetPlayingAbilityTag(TwoMinGameplayTag::Player_Ability_LockOn);
	if (!Ability)
	{
		return;
	}
	
	UTwoMinGA_LockOn_Player* LockOnAbility = Cast<UTwoMinGA_LockOn_Player>(Ability);
	if (!LockOnAbility)
	{
		return;
	}
	
	AActor* Target = LockOnAbility->GetCurrentLockOnTarget();
	if (!Target)
	{
		return;
	}
	
	FRotator LockOnRotator = (Target->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal().Rotation();
	PlayerCharacter->SetActorRotation(LockOnRotator);
	// MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("LastRollDirection"),
	// 	PlayerCharacter->GetActorLocation(), LockOnRotator);
}
