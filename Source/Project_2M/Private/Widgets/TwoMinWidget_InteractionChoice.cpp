// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_InteractionChoice.h"

#include "TwoMinDebugHelper.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Components/WrapBox.h"
#include "System/InteractionActor_NPC.h"
#include "Widgets/TwoMinWidget_IC_Slot.h"

void UTwoMinWidget_InteractionChoice::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!ChoiceBox) return;
	
	TArray<UWidget*> SlotGroup = ChoiceBox->GetAllChildren();
	if (SlotGroup.IsEmpty()) return;

	for (UWidget* ChoiceSlot : SlotGroup)
	{
		if (UTwoMinWidget_IC_Slot* CastSlot = Cast<UTwoMinWidget_IC_Slot>(ChoiceSlot))
		{
			ChoiceSlots.Add(CastSlot);
		}
	}
}

FReply UTwoMinWidget_InteractionChoice::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InKey = InKeyEvent.GetKey();
	if (InKey == EKeys::Enter || InKey == EKeys::F || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
		ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
		if (!PlayerCharacter) return FReply::Unhandled();
	
		UPlayerUIComponent* HeroUIComponent = PlayerCharacter->GetPlayerUIComponent();
		if (!HeroUIComponent) return FReply::Unhandled();
		if (!PlayerCharacter->GetInteractionActor()) return FReply::Unhandled();
	
		AInteractionActor_NPC* NPC = Cast<AInteractionActor_NPC>(PlayerCharacter->GetInteractionActor());
		if (!NPC) return FReply::Unhandled();
		if (NPC->GetCompleteInteractionText() == false) return FReply::Unhandled();
		
		// 상점.
		if (CurrentFocusIndex == 0)
		{
			HeroUIComponent->OnSetInteractionChoice.Broadcast(NPC->GetNPCType(), false);	
			PlayerCharacter->GetPlayerUIComponent()->OpenStoreWidget(PlayerCharacter, NPC, true);
			return FReply::Handled();
		}
		
		// 대화 종료
		if (CurrentFocusIndex == 1)
		{
			HeroUIComponent->OnSetInteractionChoice.Broadcast(NPC->GetNPCType(), false);		
			PlayerCharacter->OnIgnoreInputProcess(false);
			NPC->ResetInteractionProcess();
			
			return FReply::Handled();
		}
		
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Escape || InKey == EKeys::Gamepad_FaceButton_Right)
	{
		ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
		if (!PlayerCharacter) return FReply::Unhandled();
	
		UPlayerUIComponent* HeroUIComponent = PlayerCharacter->GetPlayerUIComponent();
		if (!HeroUIComponent) return FReply::Unhandled();
		if (!PlayerCharacter->GetInteractionActor()) return FReply::Unhandled();
		
		AInteractionActor_NPC* NPC = Cast<AInteractionActor_NPC>(PlayerCharacter->GetInteractionActor());
		if (!NPC) return FReply::Unhandled();
		
		HeroUIComponent->OnSetInteractionChoice.Broadcast(NPC->GetNPCType(), false);		
		PlayerCharacter->OnIgnoreInputProcess(false);
		NPC->ResetInteractionProcess();
		
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Right || InKey == EKeys::D || InKey == EKeys::Gamepad_LeftStick_Right)
	{
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Left || InKey == EKeys::A || InKey == EKeys::Gamepad_LeftStick_Left)
	{
		return FReply::Unhandled();
	}
	
	if (InKey == EKeys::Up || InKey == EKeys::W || InKey == EKeys::Gamepad_LeftStick_Up)
	{
		int32 NextInventoryIndex = CurrentFocusIndex - 1;
		if (NextInventoryIndex < 0) return FReply::Unhandled();
		if (ChoiceSlots[NextInventoryIndex]->IsLocked()) return FReply::Unhandled();
		
		CurrentFocusIndex = NextInventoryIndex;
		OnFocusSlot();
		
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Down || InKey == EKeys::S || InKey == EKeys::Gamepad_LeftStick_Down)
	{
		int32 NextInventoryIndex = CurrentFocusIndex + 1;
		if (NextInventoryIndex > ChoiceSlots.Num() - 1) return FReply::Unhandled();
		if (ChoiceSlots[NextInventoryIndex]->IsLocked()) return FReply::Unhandled();
		
		CurrentFocusIndex = NextInventoryIndex;
		OnFocusSlot();
		
		return FReply::Handled();
	}
	
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

void UTwoMinWidget_InteractionChoice::InitInteractionChoice(ENPCType InNPCType)
{
	switch (InNPCType) {
	case ENPCType::None:
		CurrentFocusIndex = 1;
		for (auto ChoiceSlot : ChoiceSlots)
		{
			if (ChoiceSlot->SlotNPCType != ENPCType::None)
			{
				ChoiceSlot->SetLocked(true);
			}
		}
		break;
	case ENPCType::Store:
		CurrentFocusIndex = 0;
		for (auto ChoiceSlot : ChoiceSlots)
		{
			ChoiceSlot->SetLocked(false);
		}
		break;
	}
	
	OnFocusSlot();
}

void UTwoMinWidget_InteractionChoice::OnFocusSlot()
{
	for (int32 Index = 0; Index < ChoiceSlots.Num(); ++Index)
	{
		if (Index == CurrentFocusIndex)
		{
			ChoiceSlots[Index]->SetFocusSlot(true);
			continue;
		}
		
		ChoiceSlots[Index]->SetFocusSlot(false);
	}

}
