// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Enemy/TwoMinEGA_GroggyBase.h"

#include "TwoMinGameplayTag.h"
#include "AbilitySystem/Ability/Player/TwoMinGA_PerfectGuard_Player.h"
#include "Character/TwoMinEnemyCharacter.h"

void UTwoMinEGA_GroggyBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo());
	if (!EnemyCharacter)
	{
		CustomCancelAbility();
		return;
	}
	
	if (!GroggyAnimMontage)
	{
		CustomCancelAbility();
		return;
	}

	StartOutline(EnemyCharacter);
	WaitGameplayEvent(FinishOutlineEventTag, true);
	PlayToAnimMontage(GroggyAnimMontage);
	EnemyCharacter->EnableExecutionWidget(true);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTwoMinEGA_GroggyBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(GetOwningActorFromActorInfo()))
	{
		FinishOutline(EnemyCharacter);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTwoMinEGA_GroggyBase::CustomEventReceived(FGameplayEventData Payload)
{
	Super::CustomEventReceived(Payload);
	
	ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(GetAvatarActorFromActorInfo());
	if (!EnemyCharacter) return;
	
	UTwoMinFunctionLibrary::AddGameplayTagToActor(EnemyCharacter, TwoMinGameplayTag::Enemy_State_DecreaseGroggy);
	GetWorld()->GetTimerManager().ClearTimer(OutlineFadeOutTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(OutlineFadeOutTimerHandle, this, 
		&UTwoMinEGA_GroggyBase::FadeOutOutline, 0.01f, true);
}

void UTwoMinEGA_GroggyBase::StartOutline(const ATwoMinEnemyCharacter* EnemyCharacter)
{
	if (!EnemyCharacter->GetMesh() || !OutlineMaterial) return;
	
	if (!OutlineDynamicMaterial)
	{
		OutlineDynamicMaterial = UMaterialInstanceDynamic::Create(OutlineMaterial, this);	
	}
		
	OutlineDynamicMaterial->SetVectorParameterValue(TEXT("LineColor"), FLinearColor(LineColor));
	OutlineDynamicMaterial->SetScalarParameterValue(TEXT("Outline"), ThicknessValue);
	EnemyCharacter->GetMesh()->SetOverlayMaterial(OutlineDynamicMaterial);
}

void UTwoMinEGA_GroggyBase::FadeOutOutline()
{
	if (!OutlineDynamicMaterial) return;
		
	float CurrentOutlineOpacity = 0;
	OutlineDynamicMaterial->GetScalarParameterValue(TEXT("Outline"), CurrentOutlineOpacity);
		
	CurrentOutlineOpacity -= 0.03f;
	OutlineDynamicMaterial->SetScalarParameterValue(TEXT("Outline"), CurrentOutlineOpacity);

	if (CurrentOutlineOpacity > 0.0f) return;
	
	if (ATwoMinEnemyCharacter* EnemyCharacter = Cast<ATwoMinEnemyCharacter>(GetOwningActorFromActorInfo()))
	{
		FinishOutline(EnemyCharacter);	
	}
}


void UTwoMinEGA_GroggyBase::FinishOutline(const ATwoMinEnemyCharacter* EnemyCharacter)
{
	if (!EnemyCharacter || !EnemyCharacter->GetMesh()) return;
			
	EnemyCharacter->EnableExecutionWidget(false);
	EnemyCharacter->GetMesh()->SetOverlayMaterial(nullptr);
	GetWorld()->GetTimerManager().ClearTimer(OutlineFadeOutTimerHandle);
}

