// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HitBox/BoxHitCollision.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/Ability/TwoMinGA_GuardBase.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinEnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

ABoxHitCollision::ABoxHitCollision()
{
	HitCollisionType = EHitCollisionType::Box;
}

void ABoxHitCollision::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (bDrawEditorDebug)
	{
#if WITH_EDITOR
		
		FQuat Rotation = GetActorQuat();
		FVector Center = GetActorLocation() + Rotation.RotateVector(Offset);

		DrawDebugBox(
			GetWorld(),
			Center,
			BoxExtent,
			Rotation,
			FColor::Green,
			false,
			3.f,
			0,
			2.f
		);
#endif	
	}
}

void ABoxHitCollision::BeginPlay()
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

void ABoxHitCollision::OnCheckHitActorCollision()
{
	Super::OnCheckHitActorCollision();
	
	TArray<FHitResult> HitResult;
	FRotator Rotation = GetActorRotation();
	FVector Center = GetActorLocation() + Rotation.RotateVector(Offset);
	
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetWorld(),
		Center,
		Center,
		BoxExtent,
		Rotation,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	
	for (FHitResult& HitResultValue : HitResult)
	{
		AActor* HitActor = HitResultValue.GetActor();
		if (!HitActor) continue;
		
		ATwoMinBaseCharacter* TargetCharacter = Cast<ATwoMinBaseCharacter>(HitActor);
		if (!TargetCharacter) continue;
		
		if (UTwoMinFunctionLibrary::IsTargetPawnHostile(OwnerCharacter, TargetCharacter) == false) continue;
		
		if (IgnoreActors.Contains(TargetCharacter)) continue;
		
		IgnoreActors.AddUnique(TargetCharacter);
		OnDamageToHitActor(TargetCharacter);
	}
}

void ABoxHitCollision::OnDamageToHitActor(AActor* HitActor)
{
	Super::OnDamageToHitActor(HitActor);
	
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
