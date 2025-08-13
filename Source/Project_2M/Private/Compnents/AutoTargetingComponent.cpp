// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/AutoTargetingComponent.h"

#include "TwoMinDebugHelper.h"
#include "Camera/CameraComponent.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UAutoTargetingComponent::StartAutoTargeting()
{
	ATwoMinPlayerCharacter* PlayerCharacter = GetOwningPawn<ATwoMinPlayerCharacter>();
	if (!PlayerCharacter)
	{
		return;
	}

	FVector InputForward = PlayerCharacter->GetInputDirection().GetSafeNormal();
	if (InputForward.IsNearlyZero())
	{
		InputForward = PlayerCharacter->GetCamera()->GetForwardVector().GetSafeNormal2D();
	}

	TArray<AActor*> TargetingGroup;
	const TArray<AActor*> IgnoreActors;
	
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		PlayerCharacter->GetActorLocation(),
		TargetingData.TargetingRange,
		ObjectTypes,
		ATwoMinEnemyCharacter::StaticClass(),
		IgnoreActors,
		TargetingGroup
	);

	CurrentTargetingActor = IsTargetingCondition(PlayerCharacter, TargetingGroup, InputForward);

	if (!CurrentTargetingActor)
	{
		//DebugTwoMin::Print(TEXT("Null Targeting Actor"), FColor::Red, 0);
		return;
	}

	// DebugTwoMin::Print(FString::Printf(TEXT("Target : %s"), *CurrentTargetingActor->GetActorLabel()),
	// 	FColor::Green, 0);
}

void UAutoTargetingComponent::EndAutoTargeting()
{
	CurrentTargetingActor = nullptr;
	//DebugTwoMin::Print(TEXT("End Auto Targeting"), FColor::Red, 1);
}

ATwoMinEnemyCharacter* UAutoTargetingComponent::IsTargetingCondition(ATwoMinPlayerCharacter* PlayerCharacter,
                                                                     TArray<AActor*> Actors, const FVector& InputForward) const
{
	AActor* CheckTargetActor = nullptr;
	int TotalScore = 0;
	
	DrawDebug(PlayerCharacter->GetActorLocation(), InputForward);
	
	for (AActor* Target : Actors)
	{
		if (!Target) continue;

		FVector ToTarget = (Target->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal();
		float Dot = FVector::DotProduct(InputForward, ToTarget);
		float AngleDegrees = FMath::RadiansToDegrees(acosf(Dot));
		float Distance = FVector::Distance(PlayerCharacter->GetActorLocation(), Target->GetActorLocation());

		// 특정 각도 안
		if (AngleDegrees > TargetingData.TargetingAngle) continue;
		
		int CurrentScore = GetScoreCalculation(AngleDegrees, TargetingData.TargetingAngle / 2);
		CurrentScore += GetScoreCalculation(Distance, TargetingData.TargetingRange / 2);
		
		if (TotalScore > CurrentScore) continue;

		TotalScore = CurrentScore;
		CheckTargetActor = Target;
	}

	return Cast<ATwoMinEnemyCharacter>(CheckTargetActor);
}

void UAutoTargetingComponent::DrawDebug(const FVector& StartLocation, const FVector& InputForward) const
{
	if (bIsDebugTargetingRange == false) return;

	DrawDebugSphere(GetWorld(), StartLocation, TargetingData.TargetingRange,32, FColor::Red,
		false,2.0f, 0, 2.0f);	

	FRotator InputRot = InputForward.Rotation();
	float Dist = TargetingData.TargetingRange;
	
	// 입력 방향.
	DrawDebugDirectionalArrow(GetWorld(), StartLocation, StartLocation + InputForward * Dist,
		100.0f, FColor::Red, false, 2.0f, 0, 5.0f
	);

	FRotator LeftRot = InputRot;
	LeftRot.Yaw -= TargetingData.TargetingAngle;
	FVector LeftDir = LeftRot.Vector();
	
	DrawDebugDirectionalArrow(GetWorld(), StartLocation, StartLocation + LeftDir * Dist,
		100.0f, FColor::Yellow, false, 2.0f, 0, 5.0f
	);

	FRotator RightRot = InputRot;
	RightRot.Yaw += TargetingData.TargetingAngle;
	FVector RightDir = RightRot.Vector();
	
	DrawDebugDirectionalArrow(GetWorld(), StartLocation, StartLocation + RightDir * Dist,
		100.0f, FColor::Yellow, false, 2.0f, 0, 5.0f
	);
}

int UAutoTargetingComponent::GetScoreCalculation(const float CurrentValue, const int MinValue) const
{
	int MaxScore = TargetingData.TargetingTotalScore / 2;
	
	for (int ScoreIndex = MaxScore; ScoreIndex > 0; --ScoreIndex)
	{
		if (CurrentValue >= MinValue * ScoreIndex)
		{
			return MaxScore - ScoreIndex;
		}
	}
	
	return MaxScore;
}
