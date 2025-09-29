// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTSE_UpdatePlayerChecker.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TwoMinBaseCharacter.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ToMinTypes/TwoMinBlackboardKeys.h"


void UBTSE_UpdatePlayerChecker::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BB = GetBlackboardComponent(OwnerComp);
	if (!BB) return;
	
	UEnemyCombatComponent* EnemyCombatComponent = GetEnemyCombatComponent(OwnerComp);
	if (!EnemyCombatComponent) return;

	if (EnemyCombatComponent->IsBattlePossible() == false)
	{
		BB->SetValueAsObject(TwoMinBBKeys::BattleTarget, nullptr);
	}

	ATwoMinBaseCharacter* Player = UpdatePlayerChecker(EnemyCombatComponent->GetOwner());
	if (Player)
	{
		BB->SetValueAsObject(TwoMinBBKeys::BattleTarget, Player);
	}
	else
	{
		BB->SetValueAsObject(TwoMinBBKeys::BattleTarget, nullptr);
	}
}

ATwoMinBaseCharacter* UBTSE_UpdatePlayerChecker::UpdatePlayerChecker(AActor* EnemyCharacter) const
{
	TArray<AActor*> Overlapped;
	TArray<AActor*> IgnoreActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	UKismetSystemLibrary::SphereOverlapActors(
            GetWorld(),
            EnemyCharacter->GetActorLocation(),
            CheckRange,
            ObjectTypes,              // 어떤 오브젝트 타입만 찾을지 (Pawn/PhysicsBody/WorldDynamic 등)
            ATwoMinPlayerCharacter::StaticClass(),
            IgnoreActors,
            Overlapped
    );

	ATwoMinBaseCharacter* PlayerCharacter = nullptr;
	for (AActor* Actor : Overlapped)
	{
		if (ATwoMinPlayerCharacter* Player = Cast<ATwoMinPlayerCharacter>(Actor))
		{
			PlayerCharacter = Player;
		}
	}
	
	return PlayerCharacter;
}
