// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/InteractionActorBase.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "InteractionActor_NPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_2M_API AInteractionActor_NPC : public AInteractionActorBase
{
	GENERATED_BODY()
	
public:
	virtual void Interact(ATwoMinPlayerCharacter* PlayerCharacter) override;
	
	virtual void ResetInteractionProcess() override;
	virtual bool IsHiddenCondition() override;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

private:
	FNPCData* GetNPCInteractionText() const;
	
	FNPCData* NPCData;
	
	UPROPERTY(EditDefaultsOnly, Category = "NPCInfo|NPC_ID")
	int32 NPC_ID;
	
	UPROPERTY(EditDefaultsOnly, Category = "NPCInfo|NPCType")
	ENPCType NPCType;
	
	UPROPERTY(EditDefaultsOnly, Category = "NPCInfo|NPC_Data")
	UDataTable* NPCDataTable;
	
	// 대화 진행율
	UPROPERTY(VisibleAnywhere, Category = "NPCInfo|NPC_InteractionProgressCount")
	int32 CurProgressCount;
	
	// 대화 종류
	UPROPERTY(VisibleAnywhere, Category = "NPCInfo|NPC_InteractionProgressCount")
	int32 CurKindCount;
	
	UPROPERTY()
	TMap<int32, int32> MaxInteractionTextGroup;
	
	UPROPERTY()
	bool bIsStartInteractionText = false;
	
	UPROPERTY()
	bool bIsCompleteInteractionText = false;
	
public:
	FORCEINLINE ENPCType GetNPCType() const { return NPCType; }
	FORCEINLINE bool GetCompleteInteractionText() const { return bIsCompleteInteractionText; }
	FORCEINLINE TSet<int32> GetStoreList() const { return NPCData->NPC_StoreItemIds; };
	FORCEINLINE FString GetNPCDealTextStr(EStoreDealTextType InStoreDealSuccessType) const
	{
		if (NPCData->NPC_StoreDealTextGroup.IsEmpty()) return TEXT("");
		if (NPCData->NPC_StoreDealTextGroup.Contains(InStoreDealSuccessType) == false) return TEXT("");
		
		return NPCData->NPC_StoreDealTextGroup[InStoreDealSuccessType];
	};
};
