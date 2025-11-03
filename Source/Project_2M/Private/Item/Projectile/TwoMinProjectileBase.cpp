
#include "Item/Projectile/TwoMinProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ATwoMinProjectileBase::ATwoMinProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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

	InitialLifeSpan = LifeTime;
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
