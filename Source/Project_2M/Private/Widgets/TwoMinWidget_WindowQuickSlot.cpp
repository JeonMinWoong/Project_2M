// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_WindowQuickSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ToMinTypes/TwoMinStructTypes.h"

void UTwoMinWidget_WindowQuickSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	QuickSlotRegisterItems.Empty();
	ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
	ItemCountText->SetVisibility(ESlateVisibility::Hidden);
	ShowRegisterText(0);
}

void UTwoMinWidget_WindowQuickSlot::SetCurrentSlotIndex(int32 InIndex)
{
	CurrentSlotIndex = InIndex;
	UpdateQuickSlotWidget(InIndex);
	PlayUISound(EUISoundType::Focus_Move);
}

void UTwoMinWidget_WindowQuickSlot::SetQuickSlotRegisterItems(FItemInstance QuickSlotRegisterItem, int32 SlotIndex,
                                                              bool bIsRegister)
{
	if (bIsRegister)
	{
		QuickSlotRegisterItems.Emplace(SlotIndex, QuickSlotRegisterItem);	
	}
	else
	{
		QuickSlotRegisterItems.Remove(SlotIndex);
	}
	
	UpdateQuickSlotWidget(SlotIndex);
}

void UTwoMinWidget_WindowQuickSlot::UpdateQuickSlotWidget(int32 SlotIndex)
{
	if (CurrentSlotIndex != SlotIndex) return;
	
	ShowRegisterText(SlotIndex);
	
	if (QuickSlotRegisterItems.Find(SlotIndex) == nullptr)
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
		ItemCountText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	FItemInstance& CurSlotItem = QuickSlotRegisterItems[SlotIndex];
	
	ItemIconImage->SetBrushFromTexture(CurSlotItem.ItemTexture);
	ItemIconImage->SetVisibility(ESlateVisibility::Visible);
	
	FText HoldCount = FText::AsNumber(CurSlotItem.HoldCount);
	ItemCountText->SetText(HoldCount);
	ItemCountText->SetVisibility(ESlateVisibility::Visible);
}

void UTwoMinWidget_WindowQuickSlot::ShowRegisterText(int32 SlotIndex)
{
	FString Str = FString::Printf(TEXT("Q%d"), SlotIndex + 1);
	FText RegisterIndex = FText::FromString(Str);
	RegisterText->SetText(RegisterIndex);
	RegisterText->SetVisibility(ESlateVisibility::Visible);
}