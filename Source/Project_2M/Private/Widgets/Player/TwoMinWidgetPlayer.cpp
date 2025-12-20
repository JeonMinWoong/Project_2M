// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Player/TwoMinWidgetPlayer.h"

#include "Compnents/UI/PlayerUIComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/BaseUIInterface.h"
#include "Widgets/TwoMinWidget_BuffWindow.h"
#include "Widgets/TwoMinWidget_GoldInfo.h"
#include "Widgets/TwoMinWidget_ItemPickUpWindow.h"
#include "Widgets/TwoMinWidget_WindowQuickSlot.h"

void UTwoMinWidgetPlayer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const IBaseUIInterface* BaseUIInterface = Cast<IBaseUIInterface>(GetOwningPlayerPawn()))
	{
		if (UPlayerUIComponent* HeroUIComponent = BaseUIInterface->GetPlayerUIComponent())
		{
			InitPlayerUIComponent(HeroUIComponent);	
		}
	}
}

void UTwoMinWidgetPlayer::InitPlayerUIComponent(UPlayerUIComponent* HeroUIComponent)
{
	HeroUIComponent->OnCurrentHealthChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentHealthPercent);
	HeroUIComponent->OnCurrentStaminaChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentStaminaPercent);
	HeroUIComponent->OnCurrentExperienceChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentExperiencePercent);
	HeroUIComponent->OnCurrentLevelChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentLevelValue);
	HeroUIComponent->OnPossiblePickUpItem.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetPossiblePickUpItem);
	HeroUIComponent->OnPossiblePickUpItem.Broadcast(false);
	HeroUIComponent->OnItemPickUpSlot.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetItemPickUpWindow);
	HeroUIComponent->OnCurrentGoldChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentGoldValue);
	HeroUIComponent->OnSetWindowQuickSlot.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::OnSetWindowQuickSlot);
	HeroUIComponent->OnSetBuffItem.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::OnSetBuffItem);
}

void UTwoMinWidgetPlayer::SetCurrentHealthPercent(float Percent)
{
	if (!HealthBar) return;

	HealthBar->SetPercent(Percent);
}

void UTwoMinWidgetPlayer::SetCurrentStaminaPercent(float Percent)
{
	if (!StaminaBar) return;

	StaminaBar->SetPercent(Percent);
}

void UTwoMinWidgetPlayer::SetCurrentExperiencePercent(float Percent)
{
	if (!ExperienceBar) return;

	ExperienceBar->SetPercent(Percent);
}

void UTwoMinWidgetPlayer::SetCurrentLevelValue(int32 InCurrentLevel)
{
	if (!LevelText) return;
	
	LevelText->SetText(FText::AsNumber(InCurrentLevel));
}

void UTwoMinWidgetPlayer::SetPossiblePickUpItem(bool bIsPossiblePickUp)
{
	if (!PickUpItem) return;

	ESlateVisibility EVisibility = bIsPossiblePickUp ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	
	PickUpItem->SetVisibility(EVisibility);
}

void UTwoMinWidgetPlayer::SetItemPickUpWindow(int32 ShowAllItem, TArray<FItemPickUpEntry>& ItemList)
{
	if (!ItemPickUpWindow) return;
	
	ItemPickUpWindow->SetVisibility(ESlateVisibility::Visible);
	ItemPickUpWindow->OnItemPickUpSlot(ShowAllItem, ItemList);
}

void UTwoMinWidgetPlayer::SetCurrentGoldValue(int32 NewGold, int32 GainGold)
{
	if (!GoldInfo) return;
	
	GoldInfo->SetGoldText(NewGold);
	GoldInfo->SetGoldNotifyText(GainGold);
}

void UTwoMinWidgetPlayer::OnSetWindowQuickSlot(FItemInstance QuickSlotRegisterItem, int32 SlotIndex, bool bIsRegister)
{
	if (!WindowQuickSlot) return;
	
	WindowQuickSlot->SetQuickSlotRegisterItems(QuickSlotRegisterItem, SlotIndex, bIsRegister);
}

void UTwoMinWidgetPlayer::OnSetBuffItem(int32 ItemID)
{
	if (!BuffWindow) return;
	
	BuffWindow->SetBuffSlot(ItemID);
}
