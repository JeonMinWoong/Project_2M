// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compnents/UI/BaseUIComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "PlayerUIComponent.generated.h"

class UTwoMinWidget_MapSelectUI;
class ATwoMinPlayerCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossiblePickUpItem, bool, bIsPossiblePickUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemPickUpSlot, int32, SaveAllItemCount, TArray<FItemPickUpEntry>&, ItemList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentGoldChanged, int32, NewGold, int32, GainGold);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSetWindowQuickSlot, FItemInstance, QuickSlotRegisterItem, 
	int32, SlotIndex, bool, bIsRegister);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetBuffItem, int32, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetAngerState, bool, bIsOnAngerMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossibleInteraction, bool, bIsPossibleInteraction);
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
	FOnStartManualSaveAnim OnStartManualSaveAnim;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI|MapSelect")
	TSubclassOf<UTwoMinWidget_MapSelectUI> MapSelectWidgetClass;
	
	UPROPERTY()
	UTwoMinWidget_MapSelectUI* MapSelectUI;
	
	UPROPERTY()
	bool bIsMapSelectWidgetOpen = false;
	
public:
	FORCEINLINE bool IsMapSelectWidgetOpen() const { return bIsMapSelectWidgetOpen; }
};
