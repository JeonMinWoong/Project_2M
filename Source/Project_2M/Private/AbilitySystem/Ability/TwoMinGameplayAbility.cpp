// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "RootMotionModifier_SkewWarp.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "AbilitySystem/Ability/TwoMinGA_AttackBase.h"
#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEGA_AttackBase.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"

class UAbilityTask_WaitGameplayEvent;

UTwoMinGameplayAbility::UTwoMinGameplayAbility()
{
	bRetriggerInstancedAbility = UTwoMinGameplayAbility::bIsReTriggerSameAbility();
}

void UTwoMinGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EToMinAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UTwoMinGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	bIsEndAbilitySendToExhaustedEvent = false;
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	bIsReTriggerAble = false;
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EToMinAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}

	PossibleCancelAbilities.Empty();

	if (bIsEndAbilitySendToExhaustedEvent)
	{
		SendToExhaustedEvent();	
	}
}

UTwoMinAbilitySystemComponent* UTwoMinGameplayAbility::GetTwoMinAbilitySystemComponentFromActorInfo() const
{
	return Cast<UTwoMinAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

bool UTwoMinGameplayAbility::bIsReTriggerSameAbility() const
{
	return false;
}

UAbilityTask_PlayMontageAndWait* UTwoMinGameplayAbility::PlayToAnimMontage(UAnimMontage* AnimMontage,
	FName StartSectionName, bool bStopWhenAbilityEnds)
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, AnimMontage, 1.f, StartSectionName, bStopWhenAbilityEnds
	);

	if (!Task) return nullptr;
	
	Task->OnCompleted.AddDynamic(this, &ThisClass::CustomCompleteAbility);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::CustomInterruptedAbility);
	Task->OnCancelled.AddDynamic(this, &ThisClass::CustomCompleteAbility);

	Task->ReadyForActivation();

	return Task;
}

void UTwoMinGameplayAbility::WaitGameplayEvent(FGameplayTag EventTag, bool bIsOnce)
{
	UAbilityTask_WaitGameplayEvent* EventTask =UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, EventTag, nullptr, bIsOnce, true);

	EventTask->EventReceived.AddDynamic(this, &ThisClass::CustomEventReceived);

	EventTask->ReadyForActivation();
}

void UTwoMinGameplayAbility::OnStartKnockBack(AActor* OwnerActor, UAnimMontage* TargetMontage, const FVector& Direction,
	const float PushDistance, const float EndTime, UCurveFloat* KnockBackCurve)
{
	ATwoMinBaseCharacter* Character = Cast<ATwoMinBaseCharacter>(OwnerActor);
	if (!Character) return;
	
	UMotionWarpingComponent* MW = Character->GetMotionWarpingComponent();
	if (!MW) return;
	
	UAnimInstance* AnimInst = Character->GetMesh() ? Character->GetMesh()->GetAnimInstance() : nullptr;
	if (!MW || !AnimInst) return;

	MW->RemoveWarpTarget(FName("KB_Target"));
	
	FVector Target;
	CalcKnockbackTarget(Character, -Direction, PushDistance, Target);
	MW->AddOrUpdateWarpTargetFromLocation(FName("KB_Target"), Target);

	const float Pos = AnimInst->Montage_GetPosition(TargetMontage);
	
	SetCurveRootMotion(TargetMontage, MW, Pos, EndTime, KnockBackCurve);
}

bool UTwoMinGameplayAbility::CalcKnockbackTarget(ATwoMinBaseCharacter* Char, const FVector& Dir,
	float Distance, FVector& OutTarget)
{
	const FVector Start = Char->GetActorLocation();
	const FVector RawTarget = Start + Dir.GetSafeNormal2D() * Distance;
	
	float R, H;
	Char->GetCapsuleComponent()->GetScaledCapsuleSize(R, H);
	
	FHitResult Hit;
	FCollisionQueryParams Q(SCENE_QUERY_STAT(KBTrace), false, Char);

	const bool bHit = Char->GetWorld()->SweepSingleByChannel(
		Hit, Start, RawTarget, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeCapsule(R, H), Q);

	if (bHit)
	{
		const float Safe = FMath::Max(0.f, (Hit.Location - Start).Size() - R - 1.f);
		OutTarget = Start + Dir.GetSafeNormal2D() * Safe;
	}
	else
	{
		OutTarget = RawTarget;
	}
	
	return true;
}

void UTwoMinGameplayAbility::SetCurveRootMotion(UAnimMontage* TargetMontage,UMotionWarpingComponent* MW,
	const float StartTime, const float EndTime, UCurveFloat* KnockBackCurve)
{
	URootMotionModifier_SkewWarp* CurMod = NewObject<URootMotionModifier_SkewWarp>(MW,
		URootMotionModifier_SkewWarp::StaticClass(), NAME_None, RF_Transient);
	
	const float MaxDuration = TargetMontage->GetPlayLength();
	const float Duration = StartTime + EndTime;

	CurMod->Animation = TargetMontage;
	CurMod->WarpTargetName = FName("KB_Target");
	CurMod->StartTime = StartTime;
	CurMod->EndTime = FMath::Clamp(Duration, StartTime + 0.05f, MaxDuration);
	CurMod->bWarpTranslation = true;
	CurMod->bIgnoreZAxis = true;
	CurMod->bWarpRotation = false;
	CurMod->AddTranslationEasingFunc = EAlphaBlendOption::Custom;
	CurMod->AddTranslationEasingCurve = KnockBackCurve;
	
	MW->AddModifier(CurMod);
}

void UTwoMinGameplayAbility::CustomEventReceived(FGameplayEventData Payload)
{
	
}

void UTwoMinGameplayAbility::OnAttackGameplayEventReceivedByMelee(FGameplayEventData Payload)
{
	AActor* InstigatorActor = const_cast<AActor*>(Payload.Instigator.Get());
	AActor* TargetActor = const_cast<AActor*>(Payload.Target.Get());
	if (!InstigatorActor || !TargetActor) return;
	
	ATwoMinBaseCharacter* BaseCharacter = Cast<ATwoMinBaseCharacter>(InstigatorActor);
	if (!BaseCharacter) return;

	const ECharacterType CharacterType = BaseCharacter->GetCharacterType();
	if (CharacterType == ECharacterType::None) return;

	UAttackPayloadObject* AttackPayload = NewObject<UAttackPayloadObject>(BaseCharacter);
	
	if (CharacterType == ECharacterType::Enemy)
	{
		//DebugTwoMin::Print(TEXT("Enemy Ability Event Received"), FColor::Red);

		UTwoMinEGA_AttackBase* EnemyAttackBase = Cast<UTwoMinEGA_AttackBase>(this);
		if (!EnemyAttackBase) return;

		const FAttackInfoData& AttackInfoData = EnemyAttackBase->GetAttackInfoData();
		AttackPayload->Data = AttackInfoData;
	}
	else if (CharacterType == ECharacterType::Player)
	{
		//DebugTwoMin::Print(TEXT("Player Ability Event Received"), FColor::Green);

		UTwoMinGA_AttackBase* PlayerAttackBase = Cast<UTwoMinGA_AttackBase>(this);
		if (!PlayerAttackBase) return;

		const FAttackInfoData& AttackInfoData = PlayerAttackBase->GetAttackInfoData();
		AttackPayload->Data = AttackInfoData;
	}

	ATwoMinBaseCharacter* TargetCharacter = Cast<ATwoMinBaseCharacter>(TargetActor);
	if (!TargetCharacter) return;
	
	Payload.OptionalObject = AttackPayload;

	bool bIsTargetGuard = AttackPayload->Data.AttackType == EAttackType::Ungaurdable ? false :
		UTwoMinFunctionLibrary::HasGameplayTag(TargetCharacter, TwoMinGameplayTag::Shared_State_Guarding);
	bool bIsTargetPerfectGuard = AttackPayload->Data.AttackType == EAttackType::Ungaurdable ? false :
		UTwoMinFunctionLibrary::HasGameplayTag(TargetCharacter, TwoMinGameplayTag::Shared_State_PerfectGuarding);

	UTwoMinGameplayAbility* Ability =
		TargetCharacter->GetAbilitySystemComponent()->GetPlayingAbilityTag(TwoMinGameplayTag::Shared_Ability_Guard);
	if (Ability)
	{
		if (UTwoMinGA_GuardBase* GuardAbility = Cast<UTwoMinGA_GuardBase>(Ability))
		{
			bool GuardSuccess = GuardAbility->IsGuardCondition(BaseCharacter, TargetCharacter);
			bIsTargetGuard = bIsTargetGuard ? GuardSuccess : false;
			bIsTargetPerfectGuard = bIsTargetPerfectGuard ? GuardSuccess : false;
		}
	}

	if (bIsTargetPerfectGuard)
	{
		UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
			TargetCharacter,
			TwoMinGameplayTag::Shared_Event_SuccessPerfectGuard,
			Payload
		);
		
		return;
	}

	DamageToEffectSpecHandle(GetAttackGameplayEffectClass(), Payload, bIsTargetGuard);
}

void UTwoMinGameplayAbility::OnAttackGameplayEventReceivedByRange(FGameplayEventData Payload)
{
	AActor* InstigatorActor = const_cast<AActor*>(Payload.Instigator.Get());
	if (!InstigatorActor) return;
	
	ATwoMinBaseCharacter* BaseCharacter = Cast<ATwoMinBaseCharacter>(InstigatorActor);
	if (!BaseCharacter) return;

	const ECharacterType CharacterType = BaseCharacter->GetCharacterType();
	if (CharacterType == ECharacterType::None) return;

	UAttackPayloadObject* AttackPayload = NewObject<UAttackPayloadObject>(BaseCharacter);
	TSubclassOf<ATwoMinProjectileBase> ProjectileBase = nullptr;
	FName SocketName = NAME_None;
	
	if (CharacterType == ECharacterType::Enemy)
	{
		//DebugTwoMin::Print(TEXT("Enemy Ability Event Received"), FColor::Red);

		UTwoMinEGA_AttackBase* EnemyAttackBase = Cast<UTwoMinEGA_AttackBase>(this);
		if (!EnemyAttackBase) return;
		
		const FAttackInfoData& AttackInfoData = EnemyAttackBase->GetAttackInfoData();
		AttackPayload->Data = AttackInfoData;

		ProjectileBase = EnemyAttackBase->GetProjectile();
		SocketName = EnemyAttackBase->GetShootSocketName();
	}
	else if (CharacterType == ECharacterType::Player)
	{
		//DebugTwoMin::Print(TEXT("Player Ability Event Received"), FColor::Green);

		UTwoMinGA_AttackBase* PlayerAttackBase = Cast<UTwoMinGA_AttackBase>(this);
		if (!PlayerAttackBase) return;

		const FAttackInfoData& AttackInfoData = PlayerAttackBase->GetAttackInfoData();
		AttackPayload->Data = AttackInfoData;

		//ProjectileBase = PlayerAttackBase->GetProjectile();
	}
	
	if (!ProjectileBase || SocketName.IsNone()) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = BaseCharacter;
	
	FVector SpawnLocation = BaseCharacter->GetMesh()->GetSocketLocation(SocketName);
	FRotator SpawnRotation = BaseCharacter->GetActorRotation();

	ATwoMinProjectileBase* Projectile = GetWorld()->SpawnActor<ATwoMinProjectileBase>(
		ProjectileBase,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (Projectile)
	{
		Projectile->SetProjectileAttackInfoData(AttackPayload->Data);
		Projectile->SetActiveAbilityTag(AbilityTags.First());
		Projectile->SetProjectileAttackGameplayEffectClass(GetAttackGameplayEffectClass());
		Projectile->SetActiveAbilityLevel(GetAbilityLevel());
	}
}

void UTwoMinGameplayAbility::OnResetAttackCountGameplayEffectReceive(FGameplayEventData Payload)
{
	
}

void UTwoMinGameplayAbility::CustomCompleteAbility()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGameplayAbility::CustomInterruptedAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
		true);
}

void UTwoMinGameplayAbility::DamageToEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,
                                                      FGameplayEventData Payload, bool bIsTargetGuard)
{
	if (!EffectClass) return;
	
	FGameplayEffectContextHandle ContextHandle = GetTwoMinAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetTwoMinAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	const UAttackPayloadObject* AttackPayload = Cast<UAttackPayloadObject>(Payload.OptionalObject);
	if (!AttackPayload)
	{
		return;
	}
	
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		TwoMinGameplayTag::Shared_SetByCaller_BaseDamage,
		AttackPayload->Data.AttackDamageCoef
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		TwoMinGameplayTag::Shared_SetByCaller_GaurdSuccess,
		bIsTargetGuard ? 1 : 0
	);

	AActor* TargetActor = const_cast<AActor*>(Payload.Target.Get());
	if (!TargetActor) return;
	
	ATwoMinBaseCharacter* TargetCharacter = Cast<ATwoMinBaseCharacter>(TargetActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetCharacter);
	
	FActiveGameplayEffectHandle ResultEffectHandle =
		GetTwoMinAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*EffectSpecHandle.Data,
		TargetASC
	);

	if (!ResultEffectHandle.WasSuccessfullyApplied())
	{
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		TargetCharacter,
		bIsTargetGuard ? TwoMinGameplayTag::Shared_Event_HitGuard : TwoMinGameplayTag::Shared_Event_HitReact,
		Payload
	);

	const float HitStopAttackerDelay = bIsTargetGuard ?
		AttackPayload->Data.HitStopAttackerData.AttackerDelay_Guard :
		AttackPayload->Data.HitStopAttackerData.AttackerDelay_Hit;
	
	const float HitStopVictimDelay = bIsTargetGuard ?
		AttackPayload->Data.HitStopVictimData.VictimDelay_Guard :
		AttackPayload->Data.HitStopVictimData.VictimDelay_Hit;
	
	HitStopProcess(Payload.Instigator->GetInstigator(), HitStopAttackerDelay);
	HitStopProcess(TargetCharacter, HitStopVictimDelay);
}

void UTwoMinGameplayAbility::HitStopProcess(AActor* HitStopCharacter, const float HitStopDelay)
{
	ATwoMinBaseCharacter* TargetCharacter = Cast<ATwoMinBaseCharacter>(HitStopCharacter);
	if (!HitStopCharacter) return;
	
	UAnimInstance* TargetAnimInstance = TargetCharacter->GetMesh()->GetAnimInstance();
	if (!TargetAnimInstance) return;
	if (HitStopDelay <= 0.f) return;

	UAnimMontage* TargetMontage = TargetAnimInstance->GetCurrentActiveMontage();
	TargetAnimInstance->Montage_Pause(TargetMontage);
	
	FTimerManager& TimerManager = TargetCharacter->GetWorldTimerManager();
	if (TimerManager.IsTimerActive(TargetCharacter->HitStopTimerHandle))
	{
		TimerManager.ClearTimer(TargetCharacter->HitStopTimerHandle);
	}
	
	TargetCharacter->GetWorldTimerManager().SetTimer(TargetCharacter->HitStopTimerHandle,
		[TargetAnimInstance, TargetMontage]()
	{
			TargetAnimInstance->Montage_Resume(TargetMontage);
	}, HitStopDelay, false);
}

TSubclassOf<UGameplayEffect> UTwoMinGameplayAbility::GetAttackGameplayEffectClass() const
{
	return nullptr;
}

float UTwoMinGameplayAbility::CalculationStaminaCost() const
{
	if (!StaminaCostCurveTable || StaminaCostName.IsNone()) return 0.f;

	int32 Level = GetAbilityLevel();
	const FString& LevelString = FString::FromInt(Level);
	const FRealCurve* Curve = StaminaCostCurveTable->FindCurve(StaminaCostName, LevelString);
	if (!Curve) return 0.f;
	
	const float StaminaCost = Curve->Eval(Level);
	if (StaminaCost <= 0.f) return 0.f;

	return StaminaCost;
}

bool UTwoMinGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
  	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const float CurrentStamina = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentStaminaAttribute());
	if (!StaminaCostCurveTable || StaminaCostName.IsNone())
	{
		return true;	
	}
	
	const bool bIsEnoughStamina = CurrentStamina > 0;
	return bIsEnoughStamina;
}

void UTwoMinGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	const float StaminaCost = CalculationStaminaCost();
	const UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (StaminaCost <= 0.f || !CostGE) return;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) return;

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(CostGE->GetClass(), GetAbilityLevel(),
		ASC->MakeEffectContext());
	
	if (!Spec.IsValid()) return;

	const float RemainStamina = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentStaminaAttribute()) - StaminaCost;
	
	Spec.Data->SetSetByCallerMagnitude(TwoMinGameplayTag::Data_Cost_Stamina_Enough, -StaminaCost);

	ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());

	if (RemainStamina < 0)
	{
		bIsEndAbilitySendToExhaustedEvent = true;
	}
}

void UTwoMinGameplayAbility::SendToExhaustedEvent() const
{
	FGameplayEventData EventData;
	EventData.Instigator = GetAvatarActorFromActorInfo();
		
	UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
		GetAvatarActorFromActorInfo(), 
		TwoMinGameplayTag::Shared_Event_Exhausted,
		EventData
	);
}

void UTwoMinGameplayAbility::CustomCancelAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
		true);
}

bool UTwoMinGameplayAbility::IsMustBeLikedToGameplayAbility() const
{
	return false;
}

bool UTwoMinGameplayAbility::IsPossibleMustBeHoldAbilityImmediatelyCancel() const
{
	return false;
}

void UTwoMinGameplayAbility::SetReTriggerActive(bool InIsReTriggerActive)
{
	bIsReTriggerAble = InIsReTriggerActive;
}

void UTwoMinGameplayAbility::AddPossibleCancelAbility(UTwoMinGameplayAbility* InAbility)
{
	PossibleCancelAbilities.AddUnique(InAbility);
}

void UTwoMinGameplayAbility::RemovePossibleCancelAbility(UTwoMinGameplayAbility* InAbility)
{
	if (PossibleCancelAbilities.IsEmpty())
	{
		return;
	}

	PossibleCancelAbilities.Remove(InAbility);
}

bool UTwoMinGameplayAbility::IsPossibleCancelAbility(UTwoMinGameplayAbility* InAbility) const
{
	return PossibleCancelAbilities.Contains(InAbility);
}


