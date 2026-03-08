
#include "Item/HitBox/HitCollisionBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/Ability/TwoMinGameplayAbility.h"
#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

class UTwoMinGA_GuardBase;

AHitCollisionBase::AHitCollisionBase()
{
	PrimaryActorTick.bCanEverTick = true;
	HitCollisionType = EHitCollisionType::None;
}

void AHitCollisionBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

void AHitCollisionBase::BeginPlay()
{
	Super::BeginPlay();
	
	ATwoMinBaseCharacter* MyOwner = Cast<ATwoMinBaseCharacter>(GetOwner());
	if (!MyOwner)
	{
		OwnerCharacter = Cast<ATwoMinBaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}
	else
	{
		OwnerCharacter = MyOwner;
	}
}

void AHitCollisionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentHitTerm >= MaxTimer)
	{
		Destroy();
		return;	
	}
	
	CurrentHitTerm += DeltaTime;
	if (CurrentHitTerm >= MaxHitTerm)
	{
		if (CurrentHitCount >= MaxHitCount) return;
		
		CurrentHitCount++;
		OnCheckHitActorCollision();
	}
}

void AHitCollisionBase::Destroyed()
{
	Super::Destroyed();
	
	IgnoreActors.Empty();
}

void AHitCollisionBase::OnCheckHitActorCollision()
{
	UTwoMinFunctionLibrary::PlaySoundAtLocation(this, HitSound, GetActorLocation());
}

void AHitCollisionBase::OnDamageToHitActor(AActor* HitActor)
{
	FGameplayEventData EventData;
	EventData.Instigator = OwnerCharacter;
	EventData.Target = HitActor;
	
	ATwoMinBaseCharacter* TargetCharacter = Cast<ATwoMinBaseCharacter>(HitActor);
	if (!OwnerCharacter || !TargetCharacter) return;
	
	bool bIsTargetGuard = AttackInfoData.AttackType == EAttackType::Ungaurdable ? false :
			UTwoMinFunctionLibrary::HasGameplayTag(TargetCharacter, TwoMinGameplayTag::Shared_State_Guarding);
	bool bIsTargetPerfectGuard = AttackInfoData.AttackType == EAttackType::Ungaurdable ? false :
		UTwoMinFunctionLibrary::HasGameplayTag(TargetCharacter, TwoMinGameplayTag::Shared_State_PerfectGuarding);

	UTwoMinGameplayAbility* Ability =
		TargetCharacter->GetAbilitySystemComponent()->GetPlayingAbilityTag(TwoMinGameplayTag::Shared_Ability_Guard);
	if (Ability)
	{
		if (UTwoMinGA_GuardBase* GuardAbility = Cast<UTwoMinGA_GuardBase>(Ability))
		{
			bool GuardSuccess = GuardAbility->IsGuardCondition(OwnerCharacter, TargetCharacter);
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
	
	UAttackPayloadObject* AttackPayload = NewObject<UAttackPayloadObject>(OwnerCharacter);
	AttackPayload->Data = AttackInfoData;
	EventData.OptionalObject = AttackPayload;
	
	UTwoMinAbilitySystemComponent* TwoMinASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!TwoMinASC) return;
	
	FGameplayEffectContextHandle ContextHandle = TwoMinASC->MakeEffectContext();
	ContextHandle.SetAbility(TwoMinASC->GetActiveAbility(ActiveAbilityTag));
	ContextHandle.AddSourceObject(OwnerCharacter);
	ContextHandle.AddInstigator(OwnerCharacter, OwnerCharacter);

	FGameplayEffectSpecHandle EffectSpecHandle = TwoMinASC->MakeOutgoingSpec(
		CollisionAttackGameplayEffectClass,
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
	
	if (OwnerCharacter->GetCharacterType() == ECharacterType::Player)
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(
			TwoMinGameplayTag::Shared_SetByCaller_GroggyAmount,
			UTwoMinFunctionLibrary::AttackTypeChangeToAmount(AttackPayload->Data.AttackType)
		);
	}
	
	FActiveGameplayEffectHandle ResultEffectHandle =
		TwoMinASC->ApplyGameplayEffectSpecToTarget(
		*EffectSpecHandle.Data,
		TargetCharacter->GetAbilitySystemComponent()
	);
	
	if (OwnerCharacter->GetCharacterType() == ECharacterType::Enemy)
	{
		UTwoMinAbilitySystemComponent* ASC = TargetCharacter->GetAbilitySystemComponent();
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
		TargetCharacter,
		bIsTargetGuard ? TwoMinGameplayTag::Shared_Event_HitGuard : TwoMinGameplayTag::Shared_Event_HitReact,
		EventData
	);
	
	UTwoMinAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;
	
	ASC->GetPlayingAbilityTag(ActiveAbilityTag)->OnHitStop(EventData, bIsTargetGuard, AttackPayload, TargetCharacter);
}

bool AHitCollisionBase::IsCustomHitCondition(AActor* HitActor)
{
	return true;
}
