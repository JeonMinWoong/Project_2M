
#include "Item/Projectile/TwoMinProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
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
	APawn* OwnerPawn = GetInstigator();
	if (!OwnerPawn)
	{
		Destroy();
		return;
	}
	
	if (!HitPawn || UTwoMinFunctionLibrary::IsTargetPawnHostile(OwnerPawn, HitPawn) == false)
	{
		Destroy();
		return;
	}

	FGameplayEventData EventData;
	EventData.Instigator = OwnerPawn;
	EventData.Target = HitPawn;

	HandleApplyProjectileDamage(HitPawn, EventData);
	Destroy();
}

void ATwoMinProjectileBase::HandleApplyProjectileDamage(APawn* HitPawn, FGameplayEventData& PayLoad) const
{
	ATwoMinBaseCharacter* BaseCharacter = Cast<ATwoMinBaseCharacter>(PayLoad.Instigator);
	ATwoMinBaseCharacter* OtherCharacter = Cast<ATwoMinBaseCharacter>(PayLoad.Target);
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
		TwoMinGameplayTag::Shared_Event_HitReact,
		PayLoad
	);
}
