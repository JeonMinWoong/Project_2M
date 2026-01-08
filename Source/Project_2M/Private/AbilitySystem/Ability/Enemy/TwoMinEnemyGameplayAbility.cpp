// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinEnemyGameplayAbility.h"

#include "TwoMinGameplayTag.h"
#include "Compnents/Combat/BaseCombatComponent.h"

void UTwoMinEnemyGameplayAbility::OnStartTeleport()
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter) return;
	
	MyCharacter->GetMesh()->SetVisibility(false, false);
	MyCharacter->GetCombatComponent()->EnableWeaponsMesh(false);
	
	UTwoMinFunctionLibrary::AddGameplayTagToActor(
		MyCharacter, 
		TwoMinGameplayTag::Shared_State_Invincible
	);
	
	// todo : 연출~
}

void UTwoMinEnemyGameplayAbility::OnFinishTeleport()
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter) return;
	
	MyCharacter->GetMesh()->SetVisibility(true, false);
	MyCharacter->GetCombatComponent()->EnableWeaponsMesh(true);
	
	UTwoMinFunctionLibrary::RemoveGameplayTagToActor(
		MyCharacter, 
	TwoMinGameplayTag::Shared_State_Invincible
	);
	
	// todo : 이동 ~
}
