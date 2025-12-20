// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_EquipmentWindow.h"

#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "AbilitySystem/TwoMinAttributeSet.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Components/RichTextBlock.h"
#include "Components/UniformGridPanel.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/ItemDataManager.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "Widgets/TwoMinWidget_InventorySlot.h"

UTwoMinWidget_EquipmentWindow::UTwoMinWidget_EquipmentWindow()
{
	InventoryWindowType = EInventoryWindowType::Equipment;
}

void UTwoMinWidget_EquipmentWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	StatusTextBlocks.Empty();
	
	TArray<UWidget*> TextBlocks = GridStatusGroup->GetAllChildren();
	for (UWidget* TextBlock : TextBlocks)
	{
		if (URichTextBlock* CastTextBlock = Cast<URichTextBlock>(TextBlock))
		{
			StatusTextBlocks.Add(CastTextBlock);
		}
	}
}

void UTwoMinWidget_EquipmentWindow::UpdateStatusText()
{
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;
	
	UTwoMinAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!ASC) return;
	
	UCurveTable* CurveTable = PlayerCharacter->GetNeedToLevelUp_ExperienceCurveTable();
	const int32 CurLevel = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetCurrentLevelAttribute());
	
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	int32 AttackPower = 0;
	int32 DefensePower = 0;
	int32 MaxHealth = 0;
	int32 MaxStamina = 0;
	TArray<TObjectPtr<UTwoMinWidget_InventorySlot>> EquipmentSlots = GetInventorySlots();
	for (auto TwoMinWidget_InventorySlot : EquipmentSlots)
	{
		FItemInstance EquipItem = TwoMinWidget_InventorySlot->GetItemInstance();
		FItemEquipmentData EquipmentData = GI->ItemDataManager->GetItemEquipmentData(EquipItem.ItemID);
		if (EquipItem.bIsRegister == false) continue;
		
		AttackPower += GetEquipmentStatusValue(EquipmentData, EStatusType::Attack);
		DefensePower += GetEquipmentStatusValue(EquipmentData, EStatusType::Defense);
		MaxHealth += GetEquipmentStatusValue(EquipmentData, EStatusType::MaxHealth);
		MaxStamina += GetEquipmentStatusValue(EquipmentData, EStatusType::MaxStamina);
	}
	
	SetStatusText(0, CurveTable, CurLevel, ASC, AttackPower);
	SetStatusText(1, CurveTable, CurLevel, ASC, DefensePower);
	SetStatusText(2, CurveTable, CurLevel, ASC, MaxHealth);
	SetStatusText(3, CurveTable, CurLevel, ASC, MaxStamina);
}

int32 UTwoMinWidget_EquipmentWindow::GetEquipmentStatusValue(const FItemEquipmentData EquipmentData,
	const EStatusType StatusType) const
{
	int32 EquipStatusValue = 0;
	if (EquipmentData.EquipmentPower.Contains(StatusType))
	{
		EquipStatusValue = EquipmentData.EquipmentPower[StatusType];
	}
	
	return EquipStatusValue;
}

void UTwoMinWidget_EquipmentWindow::SetStatusText(const int32 Index, const UCurveTable* CurveTable, const int32 CurLevel,
	UTwoMinAbilitySystemComponent* ASC, int32 EquipValue)
{
	int32 TotalValue = 0;
	int32 BaseStatusValue = 0;
	int32 EquipStatusValue = 0;
	int32 BuffStatusValue = 0;
	if (Index == 0)
	{
		TotalValue = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetAttackPowerAttribute());
		const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.AttackPower"), TEXT(""));
		BaseStatusValue = Curve->Eval(CurLevel);
		EquipStatusValue = EquipValue;
		TotalValue -= BaseStatusValue + EquipStatusValue;
		BuffStatusValue = TotalValue;
	}
	else if (Index == 1)
	{
		TotalValue = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetDefensePowerAttribute());
		const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.DefensePower"), TEXT(""));
		BaseStatusValue = Curve->Eval(CurLevel);
		EquipStatusValue = EquipValue;
		TotalValue -= BaseStatusValue + EquipStatusValue;
		BuffStatusValue = TotalValue;
	}
	else if (Index == 2)
	{
		TotalValue = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetMaxHealthAttribute());
		const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.MaxHealth"), TEXT(""));
		BaseStatusValue = Curve->Eval(CurLevel);
		EquipStatusValue = EquipValue;
		TotalValue -= BaseStatusValue + EquipStatusValue;
		BuffStatusValue = TotalValue;
	}
	else if (Index == 3)
	{
		TotalValue = ASC->GetNumericAttribute(UTwoMinAttributeSet::GetMaxStaminaAttribute());
		const FRealCurve* Curve = CurveTable->FindCurve(FName("Player.MaxStamina"), TEXT(""));
		BaseStatusValue = Curve->Eval(CurLevel);
		EquipStatusValue = EquipValue;
		TotalValue -= BaseStatusValue + EquipStatusValue;
		BuffStatusValue = TotalValue;
	}
	
	// 0 공, 1 방, 3 체, 4 지
	FString Str = FString::Printf(TEXT("%d"), BaseStatusValue);
	
	if (EquipStatusValue > 0)
	{
		Str += FString::Printf(TEXT(" <Weapon>(+%d)</>"), EquipStatusValue);
	}
	
	if (BuffStatusValue > 0)
	{
		Str += FString::Printf(TEXT(" <Buff>(+%d)</>"), BuffStatusValue);
	}
	
	FText TextValue = FText::FromString(Str);
	StatusTextBlocks[Index]->SetText(TextValue);
}
