// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_TitleUI.h"

#include "TwoMinFunctionLibrary.h"
#include "Components/UniformGridPanel.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "GameModes/TwoMinTitleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/WorldStageManager.h"
#include "Widgets/TwoMinWidget_BaseButton.h"

void UTwoMinWidget_TitleUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!TitleMenuGridPanel) return;
	
	TArray<UWidget*> ButtonGroup = TitleMenuGridPanel->GetAllChildren();
	if (ButtonGroup.IsEmpty()) return;

	for (UWidget* Button : ButtonGroup)
	{
		if (UTwoMinWidget_BaseButton* CastSlot = Cast<UTwoMinWidget_BaseButton>(Button))
		{
			MenuButtonSlots.Add(CastSlot);
		}
	}

	const bool IsExistSaveData = UTwoMinFunctionLibrary::IsExistSaveGameData();
	if (!IsExistSaveData)
	{
		MenuButtonSlots[1]->SetLocked(true);
	}
	
	CurrentFocusIndex = IsExistSaveData ? 1 : 0;
	OnFocusSlot();
}

FReply UTwoMinWidget_TitleUI::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InKey = InKeyEvent.GetKey();
	if (InKey == EKeys::Enter || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
		if (CurrentFocusIndex == 0)
		{
			UTwoMinFunctionLibrary::RemoveSaveGameData();
		}
		else if (CurrentFocusIndex == 2)
		{
			// todo : Setting Window
			return FReply::Unhandled();
		}
		else if (CurrentFocusIndex == 3)
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
			return FReply::Handled();
		}
		
		ATwoMinTitleGameMode* TitleGM = Cast<ATwoMinTitleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (!TitleGM) return FReply::Unhandled();

		const UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
		if (!GI) return FReply::Unhandled();
		
		const FName GoStageName = FName(*GI->StateManager->GetVillageName());
		TitleGM->OnEnterInGame(GoStageName);
		
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Up || InKey == EKeys::W || InKey == EKeys::Gamepad_LeftStick_Up)
	{
		int32 NextInventoryIndex = CurrentFocusIndex - 1;
		if (NextInventoryIndex < 0) return FReply::Unhandled();
		if (MenuButtonSlots[NextInventoryIndex]->IsLocked())
		{
			NextInventoryIndex += -1;
		}
		
		CurrentFocusIndex = NextInventoryIndex;
		OnFocusSlot();
		
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Down || InKey == EKeys::S || InKey == EKeys::Gamepad_LeftStick_Down)
	{
		int32 NextInventoryIndex = CurrentFocusIndex + 1;
		if (NextInventoryIndex > MenuButtonSlots.Num() - 1) return FReply::Unhandled();
		if (MenuButtonSlots[NextInventoryIndex]->IsLocked())
		{
			NextInventoryIndex += 1;
		}
		
		CurrentFocusIndex = NextInventoryIndex;
		OnFocusSlot();
		
		return FReply::Handled();
	}
	
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

void UTwoMinWidget_TitleUI::OnFocusSlot()
{
	MenuButtonSlots[CurrentFocusIndex]->SetFocus();
}

