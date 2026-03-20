// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGA_ExecutionCaster.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Ability/TwoMinAT_ExecutionAutoPosition.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/ExecutionComponent.h"
#include "Kismet/GameplayStatics.h"

FAttackInfoData& UTwoMinGA_ExecutionCaster::GetAttackInfoData()
{
	int32 ExecutionNumber = CachedExecutionData->ExecutionNumber;
	ExecutionNumber += CachedExecutionData->bIsExecutionForward ? 0 : 10;
	FExecutionInfoData* ExecInfo = ExecutionInfosData.Find(ExecutionNumber);
	FAttackInfoData* AttackInfo = ExecInfo->AttackInfosData.Find(CurComboCount[ExecutionNumber]);
	return *AttackInfo;
}

void UTwoMinGA_ExecutionCaster::AddComboCount(FGameplayEventData Payload)
{
	int32 ExecutionNumber = CachedExecutionData->ExecutionNumber;
	ExecutionNumber += CachedExecutionData->bIsExecutionForward ? 0 : 10;
	CurComboCount[ExecutionNumber] = CurComboCount[ExecutionNumber] + 1;
	if (CurComboCount[ExecutionNumber] > MaxComboCount[ExecutionNumber])
	{
		if (bIsFinalDamageToInstantDeath[ExecutionNumber])
		{
			InstantDamage(Payload);
		}
	}
	
}

void UTwoMinGA_ExecutionCaster::InstantDamage(FGameplayEventData Payload)
{
	AActor* Target = const_cast<AActor*>(Payload.Target.Get());
	if (!Target) return;

	ATwoMinEnemyCharacter* ExecutionTarget = Cast<ATwoMinEnemyCharacter>(Target);
	if (!ExecutionTarget) return;

	if (ExecutionTarget->GetMonsterType() != EMonsterType::Normal) return;
	
	UAttackPayloadObject* AttackPayload = NewObject<UAttackPayloadObject>(this);
	FAttackInfoData AttackInfoData = FAttackInfoData(EAttackType::Execution, 99999.f); 
	AttackPayload->Data = AttackInfoData;
	Payload.OptionalObject = AttackPayload;
					
	DamageToEffectSpecHandle(GetAttackGameplayEffectClass(), Payload, false, true);
}

void UTwoMinGA_ExecutionCaster::PreActivate(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	FGameplayEventData EventData;
	UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
		GetAvatarActorFromActorInfo(),
		TwoMinGameplayTag::Shared_Event_ResetAttackCount,
		EventData
	);

	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UTwoMinGA_ExecutionCaster::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	if (ExecutionForwardCasterMontages.IsEmpty() && ExecutionBackCasterMontages.IsEmpty())
	{
		CustomCancelAbility();
		return;
	}
	
	UExecutionComponent* ExecutionComponent = GetExecutionComponent();
	if (!ExecutionComponent)
	{
		CustomCancelAbility();
		return;
	}

	ATwoMinBaseCharacter* ExecutionTarget = ExecutionComponent->GetExecutionTarget();
	if (!ExecutionTarget)
	{
		CustomCancelAbility();
		return;
	}

	int32 ExecutionNumber = 0;
	bool bIsExecutionForward = ExecutionComponent->IsExecutionForward();
	UAnimMontage* ExecutionMontage = GetExecutionMontage(bIsExecutionForward, ExecutionNumber);
	if (!ExecutionMontage) return;
	
	ExecutionComponent->SetExecutionNumber(ExecutionNumber);

	AActor* MyActor = GetAvatarActorFromActorInfo();
	UExecutionPayloadObject* ExecutionPayloadObject = NewObject<UExecutionPayloadObject>(MyActor);
	ExecutionPayloadObject->bIsExecutionForward = bIsExecutionForward;
	ExecutionPayloadObject->ExecutionNumber = ExecutionNumber;

	CachedExecutionData = ExecutionPayloadObject;
	
	FGameplayEventData PayLoad;
	PayLoad.Instigator = MyActor;
	PayLoad.Target = ExecutionTarget;
	PayLoad.OptionalObject = ExecutionPayloadObject;
	
	UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
		ExecutionTarget,
		TwoMinGameplayTag::Shared_Event_Execution_Target,
		PayLoad
	);
	
	PlayCinematicEvent(MyActor, ExecutionTarget, ExecutionNumber);
	PlayToAnimMontage(ExecutionMontage);
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, OnHitExecution, nullptr, false, true);

	Task->EventReceived.AddDynamic(this, &ThisClass::OnExecutionGameplayEventReceive);
	Task->ReadyForActivation();

	StartAutoPosition();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGA_ExecutionCaster::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	UExecutionComponent* ExecutionComponent = GetExecutionComponent();
	if (!ExecutionComponent)
	{
		CustomCancelAbility();
		return;
	}

	int32 ExecutionNumber = ExecutionComponent->GetExecutionNumber();
	ExecutionNumber += ExecutionComponent->IsExecutionForward() ? 0 : 10;
	
	CurComboCount[ExecutionNumber] = 1;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

TSubclassOf<UGameplayEffect> UTwoMinGA_ExecutionCaster::GetAttackGameplayEffectClass() const
{
	return AttackGameplayEffectClass;
}

UAnimMontage* UTwoMinGA_ExecutionCaster::GetExecutionMontage(bool bIsExecutionForward, int32& ExecutionNumber)
{
	if (bIsExecutionForward)
	{
		ExecutionNumber = FMath::RandRange(1, ExecutionForwardCasterMontages.Num());
		return ExecutionForwardCasterMontages[ExecutionNumber];
	}
	
	ExecutionNumber = FMath::RandRange(1, ExecutionBackCasterMontages.Num());
	return ExecutionBackCasterMontages[ExecutionNumber];
}

void UTwoMinGA_ExecutionCaster::PlayCinematicEvent(AActor* MyActor, AActor* TargetActor, int32 ExecutionNumber)
{
	ExecutionNumber += CachedExecutionData->bIsExecutionForward ? 0 : 10;
	ATwoMinPlayerCharacter* MyCharacter = Cast<ATwoMinPlayerCharacter>(MyActor);
	if (!MyCharacter) return;
	
	if (FExecutionCinematicData.IsEmpty()) return;
	
	AActor* ExecutionDummyActor = MyCharacter->GetExecutionCinematicDummy();
	if (!ExecutionDummyActor)
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), 
			FName("ExecutionCinematicDummy"), OutActors);
		if (OutActors.Num() == 0) return;

		ExecutionDummyActor = OutActors[0];
		if (!ExecutionDummyActor) return;
		
		MyCharacter->SetExecutionCinematicDummy(ExecutionDummyActor);
	}
	
	const FVector CameraToLocation = MyCharacter->GetActorLocation();
	FRotator CameraToRotation = (TargetActor->GetActorLocation() - CameraToLocation).GetSafeNormal().Rotation();
	CameraToRotation.Yaw += 90;

	ExecutionDummyActor->SetActorLocationAndRotation(CameraToLocation, FRotator(0, CameraToRotation.Yaw, 0));
	PlayLevelSequence(MyCharacter, FExecutionCinematicData[ExecutionNumber].CinematicLevelSequence, ExecutionDummyActor,
		FExecutionCinematicData[ExecutionNumber].OriginCamConvertBlendDelay);
}

void UTwoMinGA_ExecutionCaster::StartAutoPosition()
{
	ExecutionAutoPositionTick = UTwoMinAT_ExecutionAutoPosition::CreateTickTask(this);
	if (!ExecutionAutoPositionTick)
	{
		CustomCancelAbility();
		return;
	}

	ExecutionAutoPositionTick->OnExecutionAutoPositionTick.AddUniqueDynamic(
		this,
		&UTwoMinGA_ExecutionCaster::UpdateAutoPosition
	);

	UExecutionComponent* ExecutionComponent = GetExecutionComponent();
	if (!ExecutionComponent) return;
	
	int32 ExecutionNumber = ExecutionComponent->GetExecutionNumber();
	ExecutionNumber += ExecutionComponent->IsExecutionForward() ? 0 : 10;
	
	ExecutionAutoPositionTick->MaxDuration = ExecutionInfosData[ExecutionNumber].SnapMaxDuration;
	ExecutionAutoPositionTick->ReadyForActivation();
}

void UTwoMinGA_ExecutionCaster::UpdateAutoPosition(float DeltaTime)
{
	UExecutionComponent* ExecutionComponent = GetExecutionComponent();
	if (!ExecutionComponent) return;

	ATwoMinBaseCharacter* ExecutionTarget = ExecutionComponent->GetExecutionTarget();
	if (!ExecutionTarget) return;

	AActor* MyActor = GetAvatarActorFromActorInfo();
	int32 ExecutionNumber = ExecutionComponent->GetExecutionNumber();
	ExecutionNumber += ExecutionComponent->IsExecutionForward() ? 0 : 10;
	const float SnapSpeed = ExecutionInfosData[ExecutionNumber].SnapSpeed;
	const float SnapDistance = ExecutionInfosData[ExecutionNumber].SnapDistance;

	FRotator LookAtRot;
	ExecutionTargetAutoPosition(DeltaTime, ExecutionTarget, MyActor, ExecutionNumber, SnapSpeed, LookAtRot);
	MyActorAutoPosition(DeltaTime, ExecutionTarget, MyActor, SnapSpeed, SnapDistance, LookAtRot);
}

void UTwoMinGA_ExecutionCaster::ExecutionTargetAutoPosition(float DeltaTime, ATwoMinBaseCharacter* ExecutionTarget,
	AActor* MyActor, int32 ExecutionNumber, const float SnapSpeed, FRotator& LookAtRot)
{
	const FRotator CurTargetLookAtRot = ExecutionTarget->GetActorRotation();
	LookAtRot = (MyActor->GetActorLocation() - ExecutionTarget->GetActorLocation()).Rotation();
	LookAtRot.Yaw += ExecutionNumber > 10 ? 180 : 0;
	LookAtRot.Pitch = 0;
	LookAtRot.Roll = 0;
	FRotator TargetRot = FMath::RInterpTo(CurTargetLookAtRot, LookAtRot, DeltaTime, SnapSpeed);
	
	ExecutionTarget->SetActorRotation(TargetRot);
}

void UTwoMinGA_ExecutionCaster::MyActorAutoPosition(float DeltaTime, ATwoMinBaseCharacter* ExecutionTarget,
	AActor* MyActor, const float SnapSpeed, const float SnapDistance, const FRotator LookAtRot)
{
	const FRotator CurMyLookAtRot = MyActor->GetActorRotation();
	const FRotator MyLookAtRot = (ExecutionTarget->GetActorLocation() - MyActor->GetActorLocation()).Rotation();
	const FRotator MyTargetRot = FMath::RInterpTo(CurMyLookAtRot, MyLookAtRot, DeltaTime, SnapSpeed);

	MyActor->SetActorRotation(FRotator(0.f, MyTargetRot.Yaw, 0.f));
	
	const FVector CurPos = MyActor->GetActorLocation(); 
	const FVector AutoPos = ExecutionTarget->GetActorLocation() - MyLookAtRot.Vector() * SnapDistance;
	const FVector TargetPos = FMath::VInterpTo(CurPos, AutoPos, DeltaTime, SnapSpeed);
	
	MyActor->SetActorLocation(FVector(TargetPos.X, TargetPos.Y, CurPos.Z));
}

