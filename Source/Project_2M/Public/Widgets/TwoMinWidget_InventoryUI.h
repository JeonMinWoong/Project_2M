// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TwoMinWidget_EquipmentWindow.h"
#include "TwoMinWidget_InventorySelect.h"
#include "TwoMinWidget_QuickWindow.h"
#include "Components/SizeBox.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_InventoryUI.generated.h"

class UCanvasPanel;
class UTwoMinWidget_ItemInfoPopup;
class UTwoMinWidget_InventoryWindow;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_InventoryUI : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	UTwoMinWidget_InventoryUI();
	virtual void NativeConstruct() override;
	void ResetInventoryUI();
	
	void OnFocusSlot();
	void OnEquipment(int32 ItemID);
	void OnUnEquipment(int32 ItemID);
protected:
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UTwoMinWidget_InventorySlot* FindInventorySlot(int32 ItemID);
	UTwoMinWidget_InventorySlot* FindQuickSlot(int32 ItemID);
	
	void MoveToInventorySlot(UTwoMinWidget_InventorySlot* InvenSlot, int32 NextIndex);
	void MoveToEquipmentSlot(UTwoMinWidget_InventorySlot* EquipSlot, int32 NextIndex);
	void MoveToQuickSlot(UTwoMinWidget_InventorySlot* QuickSlot, int32 NextIndex);
	
	void ShowInventorySelect(const UTwoMinWidget_InventorySlot* CurSlot, const EInventorySelectType NewInventorySelectType);
	void HideInventorySelect();
	
	void ShowItemInfoPopup(int32 CurItemID);
	void HideItemInfoPopup();
	
	FItemInstance* FindInventoryItem(int32 ItemID);
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_EquipmentWindow* EquipmentWindow;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_QuickWindow* QuickWindow;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_InventoryWindow* InventoryWindow;
	
	UPROPERTY()
	EInventoryWindowType CurInventoryWindowType;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_InventorySelect* InventorySelect;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_ItemInfoPopup* ItemInfoPopup;
	
	UPROPERTY()
	bool bIsQuickRegister;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* SaveKeyBoardBox;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* SaveGamePadBox;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SaveButtonText;
	
public:
	FORCEINLINE UTwoMinWidget_EquipmentWindow* GetEquipmentWindow() const { return EquipmentWindow; } 
	FORCEINLINE UTwoMinWidget_InventoryWindow* GetInventoryWindow() const { return InventoryWindow; } 
	FORCEINLINE UTwoMinWidget_QuickWindow* GetQuickWindow() const { return QuickWindow; } 
};
