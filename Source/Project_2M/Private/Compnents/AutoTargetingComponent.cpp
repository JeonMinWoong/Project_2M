// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/AutoTargetingComponent.h"

#include "TwoMinDebugHelper.h"
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

	if (bIsDebugTargetingRange)
	{
		DrawDebugSphere(GetWorld(), PlayerCharacter->GetActorLocation(), TargetingData.TargetingRange,
			32, FColor::Red, false,2.0f, 0, 2.0f);	
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

	CurrentTargetingActor = IsTargetingCondition(PlayerCharacter, TargetingGroup);

	if (!CurrentTargetingActor)
	{
		DebugTwoMin::Print(TEXT("Null Targeting Actor"), FColor::Red, 0);
		return;
	}

	DebugTwoMin::Print(FString::Printf(TEXT("Target : %s"), *CurrentTargetingActor->GetActorLabel()),
		FColor::Green, 0);
}

ATwoMinEnemyCharacter* UAutoTargetingComponent::IsTargetingCondition(ATwoMinPlayerCharacter* PlayerCharacter,
	TArray<AActor*> Actors) const
{

	FVector InputForward = PlayerCharacter->GetInputDirection();
	AActor* CheckTargetActor = nullptr;
	float MaxDot = -1.0f;
	
	for (AActor* Target : Actors)
	{
		if (!Target) continue;

		FVector ToTarget = (Target->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal();
		float Dot = FVector::DotProduct(InputForward, ToTarget);

		if (Dot <= MaxDot) continue;

		MaxDot = Dot;
		CheckTargetActor = Target;
	}

	// Todo: 각도 체크

	return Cast<ATwoMinEnemyCharacter>(CheckTargetActor);
}
