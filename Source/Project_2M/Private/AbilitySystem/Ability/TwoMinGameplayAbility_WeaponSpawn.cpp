// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/TwoMinGameplayAbility_WeaponSpawn.h"

#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Item/Weapon/TwoMinWeaponBase.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinGameplayAbility_WeaponSpawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetAvatarActorFromActorInfo();
		SpawnParameters.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
		SpawnParameters.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParameters.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		
		ATwoMinWeaponBase* SpawnWeapon =
			GetWorld()->SpawnActor<ATwoMinWeaponBase>(
				WeaponClass,
				FVector(),
				FRotator(),
				SpawnParameters
			);

		if (SpawnWeapon)
		{
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

			if (ATwoMinBaseCharacter* OwnerCharacter = Cast<ATwoMinBaseCharacter>(GetOwningActorFromActorInfo()))
			{
				if (UBaseCombatComponent* CombatComponent = OwnerCharacter->GetCombatComponent())
				{
					CombatComponent->RegisterSpawnedWeapon(WeaponSpawnTag, SpawnWeapon);
				}

				if (ATwoMinPlayerCharacter* Player = Cast<ATwoMinPlayerCharacter>(OwnerCharacter))
				{
					APlayerController* PC = GetWorld()->GetFirstPlayerController();
					if (PC)
					{
						ULocalPlayer* LocalPlayer = PC ->GetLocalPlayer();
						if (LocalPlayer)
						{
							UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem =
							ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
							if (EnhancedInputLocalPlayerSubsystem)
							{
								const FTwoMinPlayerWeaponData& WeaponData = SpawnWeapon->GetWeaponData();
								
								EnhancedInputLocalPlayerSubsystem->AddMappingContext(
									WeaponData.WeaponInputMappingContext,
									1
								);

								TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;
								Player->GetAbilitySystemComponent()->GrantHeroWeaponAbilities(
									WeaponData.DefaultWeaponAbilities,
									1,
									GrantedAbilityHandles
								);
							}
						}
					}
				}
			}
		}
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinGameplayAbility_WeaponSpawn::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
