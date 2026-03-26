// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/ExecutionComponent.h"

#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/Player/TwoMinGA_LockOn_Player.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

UExecutionComponent::UExecutionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UExecutionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateGroggyEnemies();
}

void UExecutionComponent::UpdateGroggyEnemies()
{
	if (bIsPlayingExecution) return;
	
	GroggyEnemies.Empty();
	
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetOwner());
	if (!MyCharacter) return;
	
	TArray<AActor*> Enemies;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	const TArray<AActor*> IgnoreActors;
	
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		MyCharacter->GetActorLocation(),
		ExecutionData.ExecutionRange,
		ObjectTypes,
		ATwoMinEnemyCharacter::StaticClass(),
		IgnoreActors,
		Enemies
	);

	for (auto Enemy : Enemies)
	{
		ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(Enemy);
		if (!EnemyCharacter) continue;
		if (UTwoMinFunctionLibrary::HasGameplayTag(EnemyCharacter, TwoMinGameplayTag::Enemy_State_Groggy) == false) continue;
		
		if (UTwoMinFunctionLibrary::HasGameplayTag(EnemyCharacter, TwoMinGameplayTag::Enemy_State_RecoveryGroggy))
		{
			GroggyEnemies.Remove(EnemyCharacter);
			continue;	
		}
		
		GroggyEnemies.AddUnique(EnemyCharacter);
	}

	if (GroggyEnemies.IsEmpty())
	{
		UTwoMinFunctionLibrary::RemoveGameplayTagToActor(MyCharacter, TwoMinGameplayTag::Shared_State_PossibleExecution);
		return;
	}
	
	ExecutionTarget = IsExecutionCondition(MyCharacter);
	if (!ExecutionTarget)
	{
		UTwoMinFunctionLibrary::RemoveGameplayTagToActor(MyCharacter, TwoMinGameplayTag::Shared_State_PossibleExecution);
		return;
	}
	
	UTwoMinFunctionLibrary::AddGameplayTagToActor(MyCharacter, TwoMinGameplayTag::Shared_State_PossibleExecution);
}

ATwoMinBaseCharacter* UExecutionComponent::IsExecutionCondition(ATwoMinBaseCharacter* MyCaster)
{
	AActor* CheckTargetActor = nullptr;
	AActor* AutoTargetActor = nullptr;
	int TotalScore = 0;
	int MaxScore = ExecutionData.ExecutionTotalScore / 2;

	if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(MyCaster))
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(PlayerCharacter, TwoMinGameplayTag::Player_State_LockOn))
		{
			UTwoMinGameplayAbility* Ability =
				PlayerCharacter->GetAbilitySystemComponent()->GetActiveAbility(TwoMinGameplayTag::Player_Ability_LockOn);
			if (Ability)
			{
				if (UTwoMinGA_LockOn_Player* LockAbility = Cast<UTwoMinGA_LockOn_Player>(Ability))
				{
					AutoTargetActor = LockAbility->GetCurrentLockOnTarget();
				}
			}
		}
	}

	if (AutoTargetActor)
	{
		if (UTwoMinFunctionLibrary::HasGameplayTag(AutoTargetActor, TwoMinGameplayTag::Enemy_State_RecoveryGroggy))
		{
			return nullptr;
		}
		
		FVector TargetForward = AutoTargetActor->GetActorForwardVector();
		FVector ToCaster = (MyCaster->GetActorLocation() - AutoTargetActor->GetActorLocation()).GetSafeNormal();
		float Dot = FVector::DotProduct(TargetForward, ToCaster);
		float AngleDegrees = FMath::RadiansToDegrees(acosf(Dot));
		float Distance = FVector::Distance(MyCaster->GetActorLocation(), AutoTargetActor->GetActorLocation());

		if (Distance > ExecutionData.ExecutionDistance)
		{
			return nullptr;
		}

		if (AngleDegrees > ExecutionData.ExecutionFrontAngle && AngleDegrees < ExecutionData.ExecutionBackAngle)
		{
			return nullptr;
		}
		
		bIsExecutionForward = AngleDegrees <= ExecutionData.ExecutionFrontAngle;
		return Cast<ATwoMinBaseCharacter>(AutoTargetActor);
	}
	
	for (AActor* Target : GroggyEnemies)
	{
		if (!Target) continue;
		
		if (UTwoMinFunctionLibrary::HasGameplayTag(Target, TwoMinGameplayTag::Enemy_State_RecoveryGroggy))
		{
			return nullptr;
		}

		FVector TargetForward = Target->GetActorForwardVector();
		FVector ToCaster = (MyCaster->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
		float Dot = FVector::DotProduct(TargetForward, ToCaster);
		float AngleDegrees = FMath::RadiansToDegrees(acosf(Dot));
		float Distance = FVector::Distance(MyCaster->GetActorLocation(), Target->GetActorLocation());

		if (Distance > ExecutionData.ExecutionDistance) continue;
		
		// 특정 각도 안
		if (AngleDegrees <= ExecutionData.ExecutionFrontAngle || AngleDegrees >= ExecutionData.ExecutionBackAngle)
		{
			int CurrentScore = GetScoreCalculation(Distance, MaxScore, ExecutionData.ExecutionDistance / MaxScore);
			if (TotalScore > CurrentScore) continue;
			
			TotalScore = CurrentScore;
			CheckTargetActor = Target;
			bIsExecutionForward = AngleDegrees <= ExecutionData.ExecutionFrontAngle;
		}
	}

	return Cast<ATwoMinBaseCharacter>(CheckTargetActor);
}

int UExecutionComponent::GetScoreCalculation(const float CurrentValue, int MaxScore, const int MinValue) const
{
	for (int ScoreIndex = MaxScore; ScoreIndex > 0; --ScoreIndex)
	{
		if (CurrentValue >= MinValue * ScoreIndex)
		{
			return MaxScore - ScoreIndex;
		}
	}
	
	return MaxScore;
}
