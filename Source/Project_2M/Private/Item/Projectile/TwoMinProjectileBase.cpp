
#include "Item/Projectile/TwoMinProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = InitialSpeed;
	ProjectileMovementComp->MaxSpeed = MaxSpeed;
	ProjectileMovementComp->Velocity = FVector(1.f, 0.f, 0.f);
	ProjectileMovementComp->ProjectileGravityScale = 0.f;
}

void ATwoMinProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);
	IgnoreActors.Emplace(GetOwner());
	ProjectileCollisionBox->IgnoreActorWhenMoving(GetOwner(), true);
}

void ATwoMinProjectileBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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

	if (IsFindActor == false)
	{
		return nullptr;
	}
	
	return EmptyLockOnTargetGroup[0];
}

void ATwoMinProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	APawn* HitPawn = Cast<APawn>(OtherActor);
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		Destroy();
		return;
	}
	
	if (!HitPawn || UTwoMinFunctionLibrary::IsTargetPawnHostile(OwnerPawn, HitPawn) == false)
	{
		if (HitPawn == OwnerPawn)
		{
			return;
		}
		
		Destroy();
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
	Destroy();
}

void ATwoMinProjectileBase::HandleApplyProjectileDamage(APawn* HitPawn, FGameplayEventData& PayLoad,
                                                        bool bIsTargetGuard) const
{
	AActor* MyActor = const_cast<AActor*>(PayLoad.Instigator.Get());
	AActor* TargetActor = const_cast<AActor*>(PayLoad.Target.Get());
	if (!MyActor || !TargetActor) return;
	
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

	if (bIsHit)
	{
		return;
	}
	
	// 여기서 이펙트 스폰
	FHitResult HitResult;
	HitResult.ImpactPoint = GetActorLocation();
	HitResult.ImpactNormal = GetActorLocation();
	
	PlayImpactEffect(HitResult);
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
