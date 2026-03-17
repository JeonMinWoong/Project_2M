// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "PlayerUIComponent.generated.h"

class AInteractionActor_NPC;
class UTwoMinWidget_StoreUI;
class UTwoMinWidget_MapSelectUI;
class ATwoMinPlayerCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossiblePickUpItem, bool, bIsPossiblePickUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemPickUpSlot, int32, SaveAllItemCount, TArray<FItemPickUpEntry>&, ItemList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentGoldChanged, int32, NewGold, int32, GainGold);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSetWindowQuickSlot, FItemInstance, QuickSlotRegisterItem, 
	int32, SlotIndex, bool, bIsRegister);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetBuffItem, int32, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetAngerState, bool, bIsOnAngerMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPossibleInteraction, EInteractionType, NewInTeractionType, bool, bIsPossibleInteraction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionText, const FString&, InteractionText, const bool, bOn);
DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FIsInteractionTexting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompleteInteractionText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSetInteractionChoice, ENPCType, InNPCType, bool, bOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartManualSaveAnim);

/**
 * 
 */
UCLASS()
class PROJECT_2M_API UPlayerUIComponent : public UBaseUIComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void OpenMapSelectWidget(const ATwoMinPlayerCharacter* PlayerCharacter, const bool bIsOpenMapSelectWidget);
	void OpenStoreWidget(ATwoMinPlayerCharacter* PlayerCharacter, AInteractionActor_NPC* NPC, const bool bIsOpenStoreWidget);
	
	UPROPERTY()
	FOnPercentChangedDelegate OnCurrentStaminaChanged;
	
	UPROPERTY()
	FOnPercentChangedDelegate OnCurrentFightChanged;

	UPROPERTY()
	FOnPercentChangedDelegate OnCurrentExperienceChanged;

	UPROPERTY()
	FOnIntChangedDelegate OnCurrentLevelChanged;

	UPROPERTY()
	FOnPossiblePickUpItem OnPossiblePickUpItem;
	
	UPROPERTY()
	FOnItemPickUpSlot OnItemPickUpSlot;
	
	UPROPERTY()
	FOnCurrentGoldChanged OnCurrentGoldChanged;
	
	UPROPERTY()
	FOnSetWindowQuickSlot OnSetWindowQuickSlot;
	
	UPROPERTY()
	FOnSetBuffItem OnSetBuffItem;
	
	UPROPERTY()
	FOnSetAngerState OnSetAngerState;
	
	UPROPERTY()
	FOnPossibleInteraction OnPossibleInteraction;
	
	UPROPERTY()
	FOnInteractionText OnInteractionText;
	
	UPROPERTY()
	FIsInteractionTexting OnIsInteractionTexting;

	UPROPERTY()
	FOnSetInteractionChoice OnSetInteractionChoice;
	
	UPROPERTY()
	FOnCompleteInteractionText OnCompleteInteractionText;
	
	UPROPERTY()
	FOnStartManualSaveAnim OnStartManualSaveAnim;
	
private:
	void InitMapSelectUI();
	void InitStoreUI();
	
	UPROPERTY(EditDefaultsOnly, Category = "UI|MapSelect")
	TSubclassOf<UTwoMinWidget_MapSelectUI> MapSelectWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_MapSelectUI* MapSelectUI;
	
	UPROPERTY()
	bool bIsMapSelectWidgetOpen = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI|MapSelect")
	TSubclassOf<UTwoMinWidget_StoreUI> StoreWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_StoreUI* StoreWidgetUI;
	
	UPROPERTY()
	bool bIsStoreWidgetOpen = false;
	
public:
	FORCEINLINE UTwoMinWidget_StoreUI* GetStoreUI() const { return StoreWidgetUI; }
	
	FORCEINLINE bool IsMapSelectWidgetOpen() const { return bIsMapSelectWidgetOpen; }
	FORCEINLINE bool IsStoreWidgetOpen() const { return bIsStoreWidgetOpen; }
};
