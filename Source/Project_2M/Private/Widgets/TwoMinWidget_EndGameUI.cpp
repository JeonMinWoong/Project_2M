// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_EndGameUI.h"

#include "Character/TwoMinPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/TwoMinWidget_BaseButton.h"

void UTwoMinWidget_EndGameUI::InitEndGameUI()
{
	EndGameButton_1->SetIsFocusable(true);
	EndGameButton_2->SetIsFocusable(true);
	
	CurrentFocusIndex = 0;
	EndGameButton_1->SetFocus();
}

void UTwoMinWidget_EndGameUI::ResetEndGameUI()
{
	CurrentFocusIndex = 0;
	EndGameButton_1->SetFocus();
}

FReply UTwoMinWidget_EndGameUI::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InKey = InKeyEvent.GetKey();
	
	if (InKey == EKeys::Enter || InKey == EKeys::F || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
		if (CurrentFocusIndex == 0)
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
			return FReply::Handled();
		}
		
		ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
		if (!PlayerCharacter) return FReply::Unhandled();
		
		EndGameButton_1->SetFocus();
		PlayerCharacter->OpenEndGameProcess();
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Gamepad_FaceButton_Right)
	{
		ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
		if (!PlayerCharacter) return FReply::Unhandled();
		
		EndGameButton_1->SetFocus();
		PlayerCharacter->OpenEndGameProcess();
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Left || InKey == EKeys::A || InKey == EKeys::Gamepad_LeftStick_Left)
	{
		CurrentFocusIndex = 0;
		EndGameButton_1->SetFocus();
		return FReply::Handled();
	}
	
	if (InKey == EKeys::Right || InKey == EKeys::D || InKey == EKeys::Gamepad_LeftStick_Right)
	{
		CurrentFocusIndex = 1;
		EndGameButton_2->SetFocus();
		return FReply::Handled();
	}
	
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}
