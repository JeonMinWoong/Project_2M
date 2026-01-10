// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinEnemyGameplayAbility.h"

#include "NavigationSystem.h"
#include "TwoMinGameplayTag.h"
#include "Compnents/Combat/BaseCombatComponent.h"
#include "Controller/TwoMinEnemyAIController.h"

void UTwoMinEnemyGameplayAbility::OnStartTeleport(float TeleportDistance)
{
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!MyCharacter) return;
	
	MyCharacter->GetMesh()->SetVisibility(false, false);
	MyCharacter->GetCombatComponent()->EnableWeaponsMesh(false);
	
	UTwoMinFunctionLibrary::AddGameplayTagToActor(
		MyCharacter, 
		TwoMinGameplayTag::Shared_State_Invincible
	);
	
	ATwoMinEnemyAIController* AI = Cast<ATwoMinEnemyAIController>(MyCharacter->GetController());
	if (!AI) return;
	
	ATwoMinBaseCharacter* Target = AI->GetBattleTargetCharacter();
	if (!Target) return;
	
	FVector AddLocation = Target->GetActorForwardVector() * TeleportDistance;
	FVector TeleportLocation = Target->GetActorLocation() + AddLocation;
	
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return;
	
	FNavLocation ProjectedLocation;
	bool bOnNavMesh = NavSys->ProjectPointToNavigation(
		TeleportLocation,
	ProjectedLocation,
	FVector(100.f, 100.f, 200.f)
	);
	
	if (!bOnNavMesh) return;
	
	FCollisionShape Capsule = FCollisionShape::MakeCapsule(100, 200);

	bool bBlocked = GetWorld()->SweepTestByChannel(
		TeleportLocation,
		TeleportLocation,
		FQuat::Identity,
		ECC_Pawn,
		Capsule
	);
	
	if (!bBlocked) return;
	
	FHitResult Hit;
	FVector Start = TeleportLocation + FVector(0,0,50);
	FVector End   = TeleportLocation - FVector(0,0,500);

	bool bHasGround = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility
	);
	
	if (!bHasGround) return;
	
	MyCharacter->SetActorLocation(TeleportLocation);
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
	
	ATwoMinEnemyAIController* AI = Cast<ATwoMinEnemyAIController>(MyCharacter->GetController());
	if (!AI) return;
	
	ATwoMinBaseCharacter* Target = AI->GetBattleTargetCharacter();
	if (!Target) return;
	
	FRotator Rotation = (Target->GetActorLocation() - MyCharacter->GetActorLocation()).GetSafeNormal().Rotation();
	MyCharacter->SetActorRotation(Rotation);
}
