
#include "Item/Projectile/TwoMinProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "System/TwoMinActorPoolSubsystem.h"

ATwoMinProjectileBase::ATwoMinProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	ProjectileCollisionBox->SetCollisionObjectType(ECC_GameTraceChannel1);
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	SetRootComponent(ProjectileCollisionBox);
	
	ProjectileOverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileOverlapSphere"));
	ProjectileOverlapSphere->SetCollisionObjectType(ECC_GameTraceChannel1);
	ProjectileOverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileOverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	ProjectileOverlapSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	ProjectileOverlapSphere->SetupAttachment(ProjectileCollisionBox);
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = InitialSpeed;
	ProjectileMovementComp->MaxSpeed = MaxSpeed;
	ProjectileMovementComp->Velocity = FVector(1.f, 0.f, 0.f);
	ProjectileMovementComp->ProjectileGravityScale = 0.f;
}

void ATwoMinProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	CachedStaticMeshComp = FindComponentByClass<UStaticMeshComponent>();
	GetComponents<UNiagaraComponent>(CachedNiagaraComponents);

	if (!bIsPooled)
	{
		SetLifeSpan(LifeTime);
		IgnoreActors.Emplace(GetOwner());
		ProjectileCollisionBox->IgnoreActorWhenMoving(GetOwner(), true);
	}
}

void ATwoMinProjectileBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateStaticMeshRotation(DeltaSeconds);
	
	if (CurHoverTime < HoverTime)
	{
		CurHoverTime += DeltaSeconds;
		ProjectileMovementComp->InitialSpeed = 0;
		ProjectileMovementComp->MaxSpeed = 0;
		ProjectileMovementComp->Velocity = FVector(0.f, 0.f, 0.f);
		return;
	}

	if (bIsHoverOut == false)
	{
		bIsHoverOut = true;
		ProjectileMovementComp->InitialSpeed = InitialSpeed;
		ProjectileMovementComp->MaxSpeed = MaxSpeed;
		
		FVector Direction = GetDirection();
		FVector ToTarget = Direction * ProjectileMovementComp->InitialSpeed;
		ProjectileMovementComp->Velocity = ToTarget;
		
		SetActorRotation(ToTarget.Rotation());
		return;
	}

	if (ProjectileType == EProjectileType::Homing)
	{
		HomingTick(DeltaSeconds);
	}
	else if (ProjectileType == EProjectileType::Falling)
	{
		FallingTick(DeltaSeconds);
	}
	else if (ProjectileType == EProjectileType::Target)
	{
		UpdateTargetTick();
	}
}

FVector ATwoMinProjectileBase::GetSpawnLocation()
{
	if (ProjectileType != EProjectileType::Location)
	{
		return FVector::ZeroVector;
	}
	
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	FRotator OwnerRotation = GetOwner()->GetActorRotation();
	
	float HalfLocalX = RandomLocation.X / 2;
	float HalfLocalY = RandomLocation.Y / 2;
	float HalfLocalZ = RandomLocation.Z / 2;
	
	FVector FinalLocation = OwnerLocation;
	FVector RandomVector = FVector(
		FMath::FRandRange(-HalfLocalX, HalfLocalX),
		FMath::FRandRange(-HalfLocalY, HalfLocalY),
		FMath::FRandRange(-HalfLocalZ, HalfLocalZ));
	
	return FinalLocation + SpawnLocation + OwnerRotation.RotateVector(RandomVector);
}

void ATwoMinProjectileBase::SetCustomMesh(UStaticMesh* NewMesh) const
{
	if (!NewMesh) return;
	if (!CachedStaticMeshComp) return;
	
	CachedStaticMeshComp->SetStaticMesh(NewMesh);
}

void ATwoMinProjectileBase::RecallProjectile()
{
	if (bIsRecallProjectile) return;
	
	ProjectileType = EProjectileType::Target;
	bIsRecallProjectile = true;
	bIsHitFloor = false;
	
	ProjectileMovementComp->ProjectileGravityScale = 0;
	ProjectileMovementComp->StopSimulating(FHitResult());
	ProjectileMovementComp->SetUpdatedComponent(RootComponent);
	
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATwoMinProjectileBase::HomingTick(float DeltaSeconds)
{
	if (bIsHomingStart == false)
	{
		CurHomingActivationDelay += DeltaSeconds;
		if (CurHomingActivationDelay >= HomingActivationDelay)
		{
			bIsHomingStart = true;
		}
	}
	else
	{
		CurHomingRetargetInterval += DeltaSeconds;
		if (CurHomingRetargetInterval >= HomingRetargetInterval)
		{
			CurHomingRetargetInterval = 0;
			HomingTarget = UpdateHomingTarget();
		}

		if (HomingTarget)
		{
			FVector ToTarget = (HomingTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FVector NewVelocity = FMath::VInterpTo(
				ProjectileMovementComp->Velocity,
				ToTarget * HomingAccelerationMagnitude,
				DeltaSeconds,
				2.f
			);

			ProjectileMovementComp->Velocity = NewVelocity;
			SetActorRotation(NewVelocity.Rotation());
		}
	}
}

AActor* ATwoMinProjectileBase::UpdateHomingTarget()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> EmptyLockOnTargetGroup;
	
	bool IsFindActor = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		HomingRange,
		ObjectTypes,
		ATwoMinBaseCharacter::StaticClass(),
		IgnoreActors,
		EmptyLockOnTargetGroup
	);

	if (IsFindActor == false) return nullptr;
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	TArray<AActor*> EnemyActors;
	for (AActor* TargetActor : EmptyLockOnTargetGroup)
	{
		APawn* TargetPawn = Cast<APawn>(TargetActor);
		if (UTwoMinFunctionLibrary::IsTargetPawnHostile(OwnerPawn, TargetPawn) == false) continue;
		
		ATwoMinBaseCharacter* TargetCharacter = Cast<ATwoMinBaseCharacter>(TargetActor);
		if (!TargetCharacter) continue;
		
		EnemyActors.Emplace(TargetActor);
	}
	
	if (EnemyActors.IsEmpty()) return nullptr;

	const FVector MyLocation = GetActorLocation();
	AActor* ClosestActor = nullptr;
	float ClosestDistSq = TNumericLimits<float>::Max();

	for (AActor* Enemy : EnemyActors)
	{
		const float DistSq = FVector::DistSquared(MyLocation, Enemy->GetActorLocation());
		if (DistSq < ClosestDistSq)
		{
			ClosestDistSq = DistSq;
			ClosestActor = Enemy;
		}
	}

	return ClosestActor;
}

void ATwoMinProjectileBase::FallingTick(float DeltaSeconds)
{
	if (bIsHitFloor) return;
	
	if (bIsFallingStart == false)
	{
		CurFallingTime += DeltaSeconds;
		if (CurFallingTime >= FallingStartTime)
		{
			bIsFallingStart = true;
		}
	}
	else
	{
		CurFallingGravity += FallingGravityCoef * DeltaSeconds;
		ProjectileMovementComp->ProjectileGravityScale = CurFallingGravity;
	}
}

void ATwoMinProjectileBase::UpdateStaticMeshRotation(float DeltaSeconds) const
{
	if (bIsHitFloor) return;
	if (bIsRotationStaticMesh == false) return;
	
	CachedStaticMeshComp->AddLocalRotation(RotationSpeed * DeltaSeconds);
}

void ATwoMinProjectileBase::UpdateTargetTick()
{
	if (!Owner)
	{
		DestroyProjectile();
		return;
	}
	
	ATwoMinBaseCharacter* OwnerCharacter = Cast<ATwoMinBaseCharacter>(Owner);
	if (!OwnerCharacter)
	{
		DestroyProjectile();
		return;
	}
	
	if (OwnerCharacter->GetCombatComponent()->GetIsAlive() == false)
	{
		DestroyProjectile();
		return;
	}
	
	FVector Direction = (Owner->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector ToTarget = Direction * ProjectileMovementComp->InitialSpeed;
	ProjectileMovementComp->Velocity = ToTarget;
	
	SetActorRotation(ToTarget.Rotation());
}

void ATwoMinProjectileBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsOverlapEvent == false) return;
	if (bIsRecallProjectile == false) return;
	
	APawn* HitPawn = Cast<APawn>(OtherActor);
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!HitPawn || !OwnerPawn) return;
	if (UTwoMinFunctionLibrary::IsTargetPawnHostile(OwnerPawn, HitPawn)) return;
	
	PickUpProjectileProcess(OwnerPawn);
}

void ATwoMinProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	if (bIsKeepHitFloorProjectile)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		if (ProjectileMovementComp->bShouldBounce)
		{
			if (HitResult.ImpactNormal.Z > 0.7f)
			{
				bIsHitFloor = true;
				ProjectileMovementComp->bShouldBounce = false;
				ProjectileMovementComp->Velocity = FVector(0.f, 0.f, 0.f);
				return;
			}
		}	
	}
	
	APawn* HitPawn = Cast<APawn>(OtherActor);
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		DestroyProjectile();
		return;
	}
	
	if (!HitPawn || UTwoMinFunctionLibrary::IsTargetPawnHostile(OwnerPawn, HitPawn) == false)
	{
		if (HitPawn == OwnerPawn)
		{
			return;
		}

		DestroyProjectile();
		return;
	}

	FGameplayEventData EventData;
	EventData.Instigator = OwnerPawn;
	EventData.Target = HitPawn;

	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(OwnerPawn);
	ATwoMinBaseCharacter* TargetCharacter = Cast<ATwoMinBaseCharacter>(HitPawn);
	if (!MyCharacter || !TargetCharacter) return;
	
	bool bIsTargetGuard = ProjectileAttackInfoData.AttackType == EAttackType::Ungaurdable ? false :
			UTwoMinFunctionLibrary::HasGameplayTag(TargetCharacter, TwoMinGameplayTag::Shared_State_Guarding);
	bool bIsTargetPerfectGuard = ProjectileAttackInfoData.AttackType == EAttackType::Ungaurdable ? false :
		UTwoMinFunctionLibrary::HasGameplayTag(TargetCharacter, TwoMinGameplayTag::Shared_State_PerfectGuarding);

	UTwoMinGameplayAbility* Ability =
		TargetCharacter->GetAbilitySystemComponent()->GetPlayingAbilityTag(TwoMinGameplayTag::Shared_Ability_Guard);
	if (Ability)
	{
		if (UTwoMinGA_GuardBase* GuardAbility = Cast<UTwoMinGA_GuardBase>(Ability))
		{
			bool GuardSuccess = GuardAbility->IsGuardCondition(MyCharacter, TargetCharacter);
			bIsTargetGuard = bIsTargetGuard ? GuardSuccess : false;
			bIsTargetPerfectGuard = bIsTargetPerfectGuard ? GuardSuccess : false;
		}
	}

	if (bIsTargetPerfectGuard)
	{
		UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
			TargetCharacter,
			TwoMinGameplayTag::Shared_Event_SuccessPerfectGuard,
			EventData
		);
		
		return;
	}

	OnHitPlayEffect(HitResult);
	HandleApplyProjectileDamage(HitPawn, EventData, bIsTargetGuard);
	DestroyProjectile();
}

void ATwoMinProjectileBase::HandleApplyProjectileDamage(APawn* HitPawn, FGameplayEventData& PayLoad,
                                                        bool bIsTargetGuard) const
{
	AActor* MyActor = const_cast<AActor*>(PayLoad.Instigator.Get());
	AActor* TargetActor = const_cast<AActor*>(PayLoad.Target.Get());
	if (!MyActor || !TargetActor) return;

	if (UTwoMinFunctionLibrary::HasGameplayTag(TargetActor, TwoMinGameplayTag::Shared_State_Invincible)) return;
	
	ATwoMinBaseCharacter* BaseCharacter = Cast<ATwoMinBaseCharacter>(MyActor);
	ATwoMinBaseCharacter* OtherCharacter = Cast<ATwoMinBaseCharacter>(TargetActor);
	UAttackPayloadObject* AttackPayload = NewObject<UAttackPayloadObject>(BaseCharacter);
	AttackPayload->Data = ProjectileAttackInfoData;
	PayLoad.OptionalObject = AttackPayload;

	UProjectilePayloadObject* ProjectilePayloadObject = NewObject<UProjectilePayloadObject>(BaseCharacter);
	const FProjectileInfoData ProjectileInfoData = FProjectileInfoData(GetActorLocation());
	ProjectilePayloadObject->Data = ProjectileInfoData;
	PayLoad.OptionalObject2 = ProjectilePayloadObject;
	
	UTwoMinAbilitySystemComponent* TwoMinASC = BaseCharacter->GetAbilitySystemComponent();
	if (!TwoMinASC) return;
	
	FGameplayEffectContextHandle ContextHandle = TwoMinASC->MakeEffectContext();
	ContextHandle.SetAbility(TwoMinASC->GetActiveAbility(ActiveAbilityTag));
	ContextHandle.AddSourceObject(BaseCharacter);
	ContextHandle.AddInstigator(BaseCharacter, BaseCharacter);

	FGameplayEffectSpecHandle EffectSpecHandle = TwoMinASC->MakeOutgoingSpec(
		ProjectileAttackGameplayEffectClass,
		AbilityLevel,
		ContextHandle
	);
	
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		TwoMinGameplayTag::Shared_SetByCaller_BaseDamage,
		AttackPayload->Data.AttackDamageCoef
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		TwoMinGameplayTag::Shared_SetByCaller_GaurdSuccess,
		bIsTargetGuard ? 1 : 0
	);
	
	FActiveGameplayEffectHandle ResultEffectHandle =
		TwoMinASC->ApplyGameplayEffectSpecToTarget(
		*EffectSpecHandle.Data,
		OtherCharacter->GetAbilitySystemComponent()
	);

	if (ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(MyActor))
	{
		UTwoMinAbilitySystemComponent* ASC = OtherCharacter->GetAbilitySystemComponent();
		if (ASC)
		{
			ASC->GiveFightValue(FName("Player.Hit.Fight"));
		}
	}
	
	if (!ResultEffectHandle.WasSuccessfullyApplied())
	{
		return;
	}
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		HitPawn,
		bIsTargetGuard ? TwoMinGameplayTag::Shared_Event_HitGuard : TwoMinGameplayTag::Shared_Event_HitReact,
		PayLoad
	);
}

void ATwoMinProjectileBase::OnHitPlayEffect(const FHitResult& HitResult)
{
	bIsHit = true;
	PlayImpactEffect(HitResult);
}

void ATwoMinProjectileBase::PlayImpactEffect(const FHitResult& HitResult) const
{
	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactEffect,
			HitResult.ImpactPoint,
			HitResult.ImpactNormal.Rotation()
		);
	}
	
	UTwoMinFunctionLibrary::PlaySoundAtLocation(this, ImpactSound, HitResult.ImpactPoint);
}

void ATwoMinProjectileBase::Destroyed()
{
	Super::Destroyed();

	// 풀링된 액터가 월드 종료로 파괴되는 경우 안전망
	if (DestroyCallback)
	{
		DestroyCallback();
		DestroyCallback = nullptr;
	}
}

FVector ATwoMinProjectileBase::GetDirection() const
{
	if (ProjectileType != EProjectileType::Location)
	{
		return GetActorForwardVector();
	}
	
	if (!CachedTargetCharacter)
	{
		return GetActorForwardVector();
	}

	const FVector Direction = (CachedTargetCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	return Direction;
}

void ATwoMinProjectileBase::PickUpProjectileProcess(AActor* OwnerActor)
{
	FGameplayEventData EventData;
	EventData.Instigator = OwnerActor;

	UTwoMinFunctionLibrary::SendToGameplayEffectEvent(
		OwnerActor,
		TwoMinGameplayTag::Player_Event_PickUpThrowWeapon,
		EventData
	);
	
	DestroyProjectile();
}

void ATwoMinProjectileBase::DestroyProjectile()
{
	if (ProjectileMovementComp->bShouldBounce) return;

	ReturnToPool();
}

void ATwoMinProjectileBase::ActivateFromPool(const FVector& Location, const FRotator& Rotation, AActor* NewOwner)
{
	bIsPooled = true;

	SetOwner(NewOwner);
	SetActorLocationAndRotation(Location, Rotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	ProjectileMovementComp->SetComponentTickEnabled(true);
	ProjectileMovementComp->SetUpdatedComponent(RootComponent);
	ProjectileMovementComp->InitialSpeed = InitialSpeed;
	ProjectileMovementComp->MaxSpeed = MaxSpeed;
	ProjectileMovementComp->Velocity = GetActorForwardVector() * InitialSpeed;
	ProjectileMovementComp->ProjectileGravityScale = 0.f;

	IgnoreActors.Emplace(NewOwner);
	ProjectileCollisionBox->IgnoreActorWhenMoving(NewOwner, true);

	for (UNiagaraComponent* NiagaraComp : CachedNiagaraComponents)
	{
		NiagaraComp->ReinitializeSystem();
	}

	GetWorldTimerManager().SetTimer(
		PoolReturnTimerHandle,
		this,
		&ATwoMinProjectileBase::ReturnToPool,
		LifeTime,
		false
	);
}

void ATwoMinProjectileBase::DeactivateToPool()
{
	if (DestroyCallback)
	{
		DestroyCallback();
		DestroyCallback = nullptr;
	}

	if (!bIsHit)
	{
		FHitResult HitResult;
		HitResult.ImpactPoint = GetActorLocation();
		HitResult.ImpactNormal = GetActorLocation();
		PlayImpactEffect(HitResult);
	}

	GetWorldTimerManager().ClearTimer(PoolReturnTimerHandle);
	SetLifeSpan(0.f);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	ProjectileMovementComp->StopMovementImmediately();
	ProjectileMovementComp->SetComponentTickEnabled(false);

	for (UNiagaraComponent* NiagaraComp : CachedNiagaraComponents)
	{
		NiagaraComp->Deactivate();
	}

	ResetProjectileState();
}

void ATwoMinProjectileBase::SetOwningPool(UTwoMinActorPoolSubsystem* InPool)
{
	OwningPoolSubsystem = InPool;
	bIsPooled = true;
}

void ATwoMinProjectileBase::ReturnToPool()
{
	if (!bIsPooled)
	{
		Destroy();
		return;
	}

	if (IsActorTickEnabled() == false && IsHidden())
	{
		return;
	}

	if (OwningPoolSubsystem)
	{
		OwningPoolSubsystem->ReleaseActor(this);
	}
	else
	{
		Destroy();
	}
}

void ATwoMinProjectileBase::ResetProjectileState()
{
	const ATwoMinProjectileBase* CDO = GetClass()->GetDefaultObject<ATwoMinProjectileBase>();

	ProjectileType = CDO->ProjectileType;
	bIsOverlapEvent = CDO->bIsOverlapEvent;

	bIsHit = false;
	bIsHoverOut = false;
	bIsFallingStart = false;
	bIsHomingStart = false;
	bIsRecallProjectile = false;
	bIsHitFloor = false;

	CurHoverTime = 0.f;
	CurFallingTime = 0.f;
	CurHomingActivationDelay = 0.f;
	CurHomingRetargetInterval = 0.f;
	CurFallingGravity = 0.f;

	HomingTarget = nullptr;
	CachedTargetCharacter = nullptr;
	ProjectileAttackInfoData = FAttackInfoData();
	ActiveAbilityTag = FGameplayTag();
	ProjectileAttackGameplayEffectClass = nullptr;
	AbilityLevel = 0;

	IgnoreActors.Empty();
	DestroyCallback = nullptr;
	SetOwner(nullptr);

	ProjectileMovementComp->bShouldBounce = CDO->ProjectileMovementComp->bShouldBounce;
}
