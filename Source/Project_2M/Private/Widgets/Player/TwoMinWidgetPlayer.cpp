// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Player/TwoMinWidgetPlayer.h"

#include "Animation/WidgetAnimation.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/BaseUIInterface.h"
#include "Kismet/GameplayStatics.h"
#include "System/InteractionActor_NPC.h"
#include "Widgets/TwoMinWidget_BuffWindow.h"
#include "Widgets/TwoMinWidget_FightBar.h"
#include "Widgets/TwoMinWidget_GoldInfo.h"
#include "Widgets/TwoMinWidget_InputKey_Notify.h"
#include "Widgets/TwoMinWidget_InteractionChoice.h"
#include "Widgets/TwoMinWidget_InteractionText.h"
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
	HeroUIComponent->OnCurrentFightChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentFightPercent);
	HeroUIComponent->OnCurrentExperienceChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentExperiencePercent);
	HeroUIComponent->OnCurrentLevelChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentLevelValue);
	HeroUIComponent->OnPossiblePickUpItem.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetPossiblePickUpItem);
	HeroUIComponent->OnPossiblePickUpItem.Broadcast(false);
	HeroUIComponent->OnItemPickUpSlot.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetItemPickUpWindow);
	HeroUIComponent->OnCurrentGoldChanged.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetCurrentGoldValue);
	HeroUIComponent->OnSetWindowQuickSlot.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::OnSetWindowQuickSlot);
	HeroUIComponent->OnSetBuffItem.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::OnSetBuffItem);
	HeroUIComponent->OnSetAngerState.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetAngerState);
	HeroUIComponent->OnPossibleInteraction.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetPossibleInteraction);
	HeroUIComponent->OnPossibleInteraction.Broadcast(EInteractionType::None, false);
	HeroUIComponent->OnInteractionText.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetInteractionText);
	HeroUIComponent->OnInteractionText.Broadcast(FString(""), false);
	HeroUIComponent->OnIsInteractionTexting.BindDynamic(this, &UTwoMinWidgetPlayer::IsInteractionTexting);
	HeroUIComponent->OnSetInteractionChoice.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::SetInteractionChoice);
	HeroUIComponent->OnSetInteractionChoice.Broadcast(ENPCType::None, false);
	HeroUIComponent->OnCompleteInteractionText.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::OnFinishInteractionText);
	HeroUIComponent->OnStartManualSaveAnim.AddUniqueDynamic(this, &UTwoMinWidgetPlayer::OnStartManualSave);
	
	if (OnStartInteractionTextAnim)
	{
		CompleteStartInteractionTextAnimEvent.BindDynamic(this, &UTwoMinWidgetPlayer::OnFinishInteractionText);
		BindToAnimationFinished(OnStartInteractionTextAnim, CompleteStartInteractionTextAnimEvent);
	}
	
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

void UTwoMinWidgetPlayer::SetCurrentFightPercent(float Percent)
{
	if (!FightBar) return;
	
	FightBar->SetPercent(Percent);
}

void UTwoMinWidgetPlayer::SetAngerState(bool bIsOnAngerMode)
{
	if (!FightBar) return;
	
	FightBar->SetOnAngerMode(bIsOnAngerMode);
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
	
	PickUpItem->OnEnable(bIsPossiblePickUp);
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

void UTwoMinWidgetPlayer::SetPossibleInteraction(EInteractionType NewInteractionType, bool bIsPossibleInteraction)
{
	if (!Interaction) return;
	
	if (NewInteractionType == EInteractionType::None)
	{
		Interaction->SetVisibility(ESlateVisibility::Hidden);
		InteractionNPC->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	ESlateVisibility EVisibility = bIsPossibleInteraction ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	if (NewInteractionType == EInteractionType::MapSelect)
	{
		Interaction->OnEnable(bIsPossibleInteraction);
		Interaction->SetVisibility(EVisibility);
	}
	else
	{
		if (InteractionText->IsVisible())
		{
			EVisibility = ESlateVisibility::Hidden;
		}
		
		InteractionNPC->OnEnable(bIsPossibleInteraction);
		InteractionNPC->SetVisibility(EVisibility);
	}
}

void UTwoMinWidgetPlayer::SetInteractionText(const FString& NewInteractionText, const bool bOn)
{
	if (!InteractionText) return;
	
	ESlateVisibility EVisibility = bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	if (bOn)
	{
		StartInteractionText();
		InteractionText->SetIsFocusable(true);
		InteractionText->SetInteractionText(NewInteractionText);
	}
	else
	{
		InteractionText->SetIsFocusable(false);
	}
	
	InteractionText->SetVisibility(EVisibility);
}

bool UTwoMinWidgetPlayer::IsInteractionTexting()
{
	if (!InteractionText) return false;
	
	return InteractionText->GetInteractionTexting();
}

void UTwoMinWidgetPlayer::SetInteractionChoice(ENPCType InNPCType, bool bOn)
{
	if (!InteractionChoice) return;
	
	ESlateVisibility EVisibility = bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	InteractionChoice->SetVisibility(EVisibility);
	
	if (bOn)
	{
		InteractionChoice->InitInteractionChoice(InNPCType);
	}
}

void UTwoMinWidgetPlayer::StartInteractionText()
{
	if (!OnStartInteractionTextAnim) return;
	
	if (IsAnimationPlaying(OnStartInteractionTextAnim))
	{
		StopAnimation(OnStartInteractionTextAnim);
	}
	
	PlayAnimation(OnStartInteractionTextAnim);
}

void UTwoMinWidgetPlayer::OnFinishInteractionText()
{
	if (!InteractionText) return;
	
	InteractionText->SetInteractionTexting(false);
	
	ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;
	
	UPlayerUIComponent* HeroUIComponent = PlayerCharacter->GetPlayerUIComponent();
	if (!HeroUIComponent) return;
	if (!PlayerCharacter->GetInteractionActor()) return;
	
	AInteractionActor_NPC* NPC = Cast<AInteractionActor_NPC>(PlayerCharacter->GetInteractionActor());
	if (!NPC) return;
	if (NPC->GetCompleteInteractionText() == false) return;
	
	HeroUIComponent->OnInteractionText.Broadcast(FString(""), false);	
	HeroUIComponent->OnSetInteractionChoice.Broadcast(NPC->GetNPCType(), true);	
}

void UTwoMinWidgetPlayer::OnStartManualSave()
{
	if (!OnStartManualSaveAnim) return;
	
	if (IsAnimationPlaying(OnStartManualSaveAnim))
	{
		StopAnimation(OnStartManualSaveAnim);
	}
	
	PlayAnimation(OnStartManualSaveAnim);
}
