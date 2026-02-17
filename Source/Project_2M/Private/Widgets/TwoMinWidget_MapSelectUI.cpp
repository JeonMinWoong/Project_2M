// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_MapSelectUI.h"

#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/PlayerUIComponent.h"
#include "Components/CanvasPanel.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "GameModes/TwoMinBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/WorldStageManager.h"
#include "Widgets/TwoMinWidget_BuffSlot.h"
#include "Widgets/TwoMinWidget_MapSelectSlot.h"

void UTwoMinWidget_MapSelectUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!ButtonCanvas) return;
	
	TArray<UWidget*> ButtonGroup = ButtonCanvas->GetAllChildren();
	if (ButtonGroup.IsEmpty()) return;

	for (UWidget* Button : ButtonGroup)
	{
		if (UTwoMinWidget_MapSelectSlot* CastSlot = Cast<UTwoMinWidget_MapSelectSlot>(Button))
		{
			CastSlot->InitSlot();
			StageButtonSlots.Add(CastSlot);
		}
	}
	
	CurrentFocusIndex = 0;
}

FReply UTwoMinWidget_MapSelectUI::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InKey = InKeyEvent.GetKey();
	if (InKey == EKeys::Enter || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
		AGameModeBase* RawGM = UGameplayStatics::GetGameMode(GetWorld());
		if (!RawGM) return FReply::Unhandled();
		
		ATwoMinBaseGameMode* GM = Cast<ATwoMinBaseGameMode>(RawGM);
		if (!GM) return FReply::Unhandled();
		
		UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
		if (!GI) return FReply::Unhandled();
		
		const FName GoStageName = FName(*GI->StateManager->GetIndexRealStageName(CurrentFocusIndex + 1));
		GM->OpenStageProcess(GoStageName);
		
		return FReply::Handled();
	}
	
	if (InKey == EKeys::P || InKey == EKeys::Gamepad_FaceButton_Right)
	{
		if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn()))
		{
			UPlayerUIComponent* PlayerUIComponent = PlayerCharacter->GetPlayerUIComponent();
			if (PlayerUIComponent->IsMapSelectWidgetOpen())
			{
				PlayerUIComponent->OpenMapSelectWidget(PlayerCharacter, false);	
			}
		}
		
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
		int32 NextInventoryIndex = CurrentFocusIndex + 1;
		if (NextInventoryIndex > StageButtonSlots.Num() - 1) return FReply::Unhandled();
		if (StageButtonSlots[NextInventoryIndex]->IsLocked()) return FReply::Unhandled();
		
		CurrentFocusIndex = NextInventoryIndex;
		OnFocusSlot();
		
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Down || InKey == EKeys::S || InKey == EKeys::Gamepad_LeftStick_Down)
	{
		int32 NextInventoryIndex = CurrentFocusIndex - 1;
		if (NextInventoryIndex < 0) return FReply::Unhandled();
		if (StageButtonSlots[NextInventoryIndex]->IsLocked()) return FReply::Unhandled();
		
		CurrentFocusIndex = NextInventoryIndex;
		OnFocusSlot();
		
		return FReply::Handled();
	}
	
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

void UTwoMinWidget_MapSelectUI::OnFocusSlot()
{
	StageButtonSlots[CurrentFocusIndex]->SetFocus();
}

void UTwoMinWidget_MapSelectUI::InitStageButtons()
{
	CurrentFocusIndex = 0;
	
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (!GI) return;
	
	if (!GI->StateManager) return;
	
	TMap<FString, bool> WorldStageMap = GI->StateManager->GetWorldStageMap();
	for (int32 Index = 0; Index < StageButtonSlots.Num(); ++Index)
	{
		bool bIsLocked = !WorldStageMap[GI->StateManager->GetIndexRealStageName(Index + 1)];
		StageButtonSlots[Index]->SetLocked(bIsLocked);
	}
}
