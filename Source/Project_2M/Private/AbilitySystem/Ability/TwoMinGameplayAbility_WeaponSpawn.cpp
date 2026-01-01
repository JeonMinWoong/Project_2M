// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGameplayAbility_WeaponSpawn.h"

#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AnimInstances/Player/TwoMinPlayerLinkedAnimLayer.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Item/Weapon/TwoMinWeaponBase.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinGameplayAbility_WeaponSpawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	OnChangeWeapon(WeaponClass, true);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGameplayAbility_WeaponSpawn::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinGameplayAbility_WeaponSpawn::OnChangeWeapon(TSubclassOf<ATwoMinWeaponBase> InWeaponClass, bool bIsFirst)
{
	if (!InWeaponClass) return;
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetAvatarActorFromActorInfo();
	SpawnParameters.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	SpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParameters.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

	ATwoMinWeaponBase* SpawnWeapon =
		GetWorld()->SpawnActor<ATwoMinWeaponBase>(
			InWeaponClass,
			FVector(),
			FRotator(),
			SpawnParameters
		);

	if (!SpawnWeapon) return;

	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepWorld,
		true);

	SpawnWeapon->AttachToComponent(
		GetOwningComponentFromActorInfo(),
		AttachmentRules,
		AttachSocketName
	);

	ATwoMinBaseCharacter* OwnerCharacter = Cast<ATwoMinBaseCharacter>(GetOwningActorFromActorInfo());
	if (!OwnerCharacter) return;

	if (bIsFirst == false)
	{
		OwnerCharacter->GetCombatComponent()->UnRegisterWeapon(WeaponSpawnTag);
	}
	
	OwnerCharacter->GetCombatComponent()->RegisterSpawnedWeapon(WeaponSpawnTag, SpawnWeapon);
	
	if (OwnerCharacter->GetCharacterType() == ECharacterType::Enemy) return;
	if (bIsFirst == false) return;
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (!LocalPlayer) return;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (!EnhancedInputLocalPlayerSubsystem) return;

	const FTwoMinPlayerWeaponData& WeaponData = SpawnWeapon->GetWeaponData();

	if (WeaponData.WeaponAnimLayerToLink)
	{
		OwnerCharacter->GetMesh()->LinkAnimClassLayers(WeaponData.WeaponAnimLayerToLink);
	}

	EnhancedInputLocalPlayerSubsystem->AddMappingContext(
		WeaponData.WeaponInputMappingContext,
		0
	);

	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;
	OwnerCharacter->GetAbilitySystemComponent()->GrantHeroWeaponAbilities(
		WeaponData.DefaultWeaponAbilities,
		1,
		GrantedAbilityHandles
	);
}
