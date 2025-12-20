// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "Widgets/TwoMinWidget_InventoryWindow.h"
#include "TwoMinWidget_EquipmentWindow.generated.h"

struct FItemEquipmentData;
class URichTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_EquipmentWindow : public UTwoMinWidget_InventoryWindow
{
	GENERATED_BODY()
	
public:
	UTwoMinWidget_EquipmentWindow();
	
	void UpdateStatusText();
protected:
	virtual void NativeOnInitialized() override;

private:
	int32 GetEquipmentStatusValue(const FItemEquipmentData EquipmentData, const EStatusType StatusType) const;
	void SetStatusText(const int32 Index, const UCurveTable* CurveTable, const int32 CurLevel, 
		UTwoMinAbilitySystemComponent* ASC, int32 EquipValue);
	
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* GridStatusGroup;
	
	UPROPERTY()
	TArray<URichTextBlock*> StatusTextBlocks;
};
