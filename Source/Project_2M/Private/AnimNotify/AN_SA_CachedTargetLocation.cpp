// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_SA_CachedTargetLocation.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "AbilitySystem/Ability/Enemy/TwoMinEGA_SpecialAttackBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"

void UAN_SA_CachedTargetLocation::FinishAbilityProcess(UTwoMinGameplayAbility* InAbility)
{
	ATwoMinBaseCharacter* TargetCharacter = nullptr;
	ATwoMinBaseCharacter* MyCharacter = Cast<ATwoMinBaseCharacter>(InAbility->GetAvatarActorFromActorInfo());
	if (MyCharacter->GetCharacterType() == ECharacterType::Enemy)
	{
		AAIController* AI = Cast<AAIController>(MyCharacter->GetController());
		if (!AI) return;
		
		UBlackboardComponent* BB = AI->GetBlackboardComponent();
		if (!BB) return;

		UObject* Object = BB->GetValueAsObject(TwoMinBBKeys::BattleTarget);
		if (!Object) return;

		ATwoMinBaseCharacter* BattleTarget = Cast<ATwoMinBaseCharacter>(Object);
		if (!BattleTarget) return;
		
		TargetCharacter = BattleTarget;
	}
	
	if (!TargetCharacter) return;
	
	FGameplayEventData EventData;
	EventData.Instigator = MyCharacter;
	EventData.Target = TargetCharacter;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		MyCharacter,
		EventTag,
		EventData
	);
}
