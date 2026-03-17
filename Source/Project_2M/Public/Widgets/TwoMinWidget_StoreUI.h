// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_StoreUI.generated.h"

class UTwoMinWidget_StoreDealText;
class UTwoMinWidget_StoreDealPopup;
class UTwoMinWidget_ItemInfoPopup;
class UTwoMinWidget_StoreSelect;
class ATwoMinPlayerCharacter;
class UTwoMinWidget_GoldInfo;
class AInteractionActor_NPC;
class UTwoMinWidget_StoreWindow;
class UTwoMinWidget_InventoryWindow;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_StoreUI : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void InitStoreUI(const AInteractionActor_NPC* NPC, const ATwoMinPlayerCharacter* PlayerCharacter);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;
	
	void ShowStoreSelect(const UTwoMinWidgetBase* CurSlot, const EStoreSelectType NewStoreSelectType);
	void HideStoreSelect();
	
	void UpdateFocusSlot();

private:
	void InitStoreSellList(const AInteractionActor_NPC* NPC) const;
	void InitStoreGold(const ATwoMinPlayerCharacter* PlayerCharacter) const;
	
	void ResetHoldDealInput();
	
	void ShowItemInfoPopup(int32 CurItemID);
	void HideItemInfoPopup();
	
	bool ShowStoreDealPopup(int32 CurItemID, EStoreDealPopupType NewStoreDealPopupType);
	void HideStoreInfoPopup();

	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_StoreWindow* StoreWindow;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_InventoryWindow* InventoryWindow;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_GoldInfo* StoreGoldInfo;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_StoreSelect* StoreSelect;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_ItemInfoPopup* ItemInfoPopup;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_StoreDealPopup* StoreDealPopup;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_StoreDealText* StoreDealText;
	
	UPROPERTY()
	EInventoryWindowType CurStoreWindowType = EInventoryWindowType::Store;
	
	UPROPERTY()
	float HoldDealInputDelay = 0.4f;
	
	UPROPERTY()
	float HoldIncreaseInputTime = 0.1f;
	
	UPROPERTY()
	int32 IncreaseInputCount = 1;
	
	UPROPERTY()
	float StartHoldDealInputTime;
	
	UPROPERTY()
	float CurHoldIncreaseInputTime;
	
	UPROPERTY()
	EStoreDealInputType StoreDealInput = EStoreDealInputType::None;
	
public:
	FORCEINLINE UTwoMinWidget_StoreWindow* GetStoreWindow() const { return StoreWindow; }
	FORCEINLINE UTwoMinWidget_InventoryWindow* GetInventoryWindow() const { return InventoryWindow; }
};
