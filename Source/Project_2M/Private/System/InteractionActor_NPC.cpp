// Fill out your copyright notice in the Description page of Project Settings.


#include "System/InteractionActor_NPC.h"

#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/PlayerUIComponent.h"

void AInteractionActor_NPC::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionType = EInteractionType::NPC;
	NPCData = GetNPCInteractionText();
	
	CurProgressCount = 0;
	CurKindCount = 1;

	for (int32 IndexMap = 1; IndexMap <=  NPCData->NPC_InteractionMap.Num(); ++IndexMap)
	{
		if (NPCData->NPC_InteractionMap.Contains(IndexMap) == false) continue;

		int32 ArrayCount = 0;
		for (int32 IndexArray = 0; IndexArray < NPCData->NPC_InteractionMap[IndexMap].InteractionTextGroup.Num(); ++IndexArray)
		{
			ArrayCount++;
		}
		
		MaxInteractionTextGroup.Emplace(IndexMap, ArrayCount);
	}
	
	bIsStartInteractionText = false;
	bIsCompleteInteractionText = false;
}

void AInteractionActor_NPC::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsStartInteractionText = false;
	bIsCompleteInteractionText = false;
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AInteractionActor_NPC::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsStartInteractionText = false;
	bIsCompleteInteractionText = false;
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AInteractionActor_NPC::ResetInteractionProcess()
{
	bIsStartInteractionText = false;
	bIsCompleteInteractionText = false;
	Super::ResetInteractionProcess();
}

void AInteractionActor_NPC::Interact(ATwoMinPlayerCharacter* PlayerCharacter)
{
	Super::Interact(PlayerCharacter);
	
	UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetPlayerUIComponent();
	if (!PlayerUIComponent)	return;
	
	if (bIsCompleteInteractionText) return;
	
	if (CurProgressCount >= MaxInteractionTextGroup[CurKindCount])
	{
		CurProgressCount = 0;
		bIsCompleteInteractionText = true;
		PlayerUIComponent->OnCompleteInteractionText.Broadcast();
		return;
	}
	
	if (PlayerUIComponent->OnIsInteractionTexting.IsBound() == false) return;
	if (PlayerUIComponent->OnIsInteractionTexting.Execute()) return;
	
	if (NPCData->NPC_InteractionMap.Contains(CurKindCount) == false) return;
	if (NPCData->NPC_InteractionMap[CurKindCount].InteractionTextGroup.IsValidIndex(CurProgressCount) == false) return;
	
	bIsStartInteractionText = true;
	PlayerCharacter->OnIgnoreInputProcess(true);
	const FString Str = NPCData->NPC_InteractionMap[CurKindCount].InteractionTextGroup[CurProgressCount];
	PlayerUIComponent->OnInteractionText.Broadcast(Str, true);
	PlayerUIComponent->OnPossibleInteraction.Broadcast(EInteractionType::NPC, false);
	
	CurProgressCount++;
}

bool AInteractionActor_NPC::IsHiddenCondition()
{
	return bIsStartInteractionText || bIsCompleteInteractionText;
}

FNPCData* AInteractionActor_NPC::GetNPCInteractionText() const
{
	TArray<FNPCData*> NPCTableGroup;
	FString DropTableGroupName = FString::FromInt(NPC_ID);
	NPCDataTable->GetAllRows(DropTableGroupName, NPCTableGroup);

	for (FNPCData* TableGroup : NPCTableGroup)
	{
		if (NPC_ID == TableGroup->NPC_ID)
		{
			return TableGroup;
		}
	}
	
	return nullptr;
}
