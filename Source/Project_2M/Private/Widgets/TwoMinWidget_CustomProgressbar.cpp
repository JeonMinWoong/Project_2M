// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_CustomProgressbar.h"

#include "TwoMinFunctionLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Widgets/TwoMinWidget_CheckBoxButton.h"

void UTwoMinWidget_CustomProgressbar::InitProgressbar(float InitPercent, bool bIsMute)
{
	HighlightFocusSlot(false);
	SetPercent(bIsMute ? 0 : InitPercent);
	ProgressBar->SetIsEnabled(false);
	MuteButton->SetCheckBox(bIsMute);
}

void UTwoMinWidget_CustomProgressbar::SetPercent(const float InPercent) const
{
	PercentText->SetText(FText::AsPercent(InPercent));
	ProgressBar->SetPercent(InPercent);
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ProgressPoint->Slot))
	{
		const float PosX = FMath::Lerp(-250, 250, InPercent);
		CanvasSlot->SetPosition(FVector2D(PosX, 0));
	}
	
	SetModifyInputText();
}

void UTwoMinWidget_CustomProgressbar::SetModifyInputText() const
{
	const bool bIsUsingGamePad = UTwoMinFunctionLibrary::IsUsingGamePad(GetWorld(), GetOwningPlayer()->GetPlatformUserId());
	const FString DecreaseKeyText = bIsUsingGamePad ? TEXT("LB") : TEXT("Q");
	const FString IncreaseKeyText = bIsUsingGamePad ? TEXT("RB") : TEXT("E");
	
	if (DecreaseInputText->GetText().ToString() == DecreaseKeyText) return;
	if (IncreaseInputText->GetText().ToString() == IncreaseKeyText) return;
	
	DecreaseInputText->SetText(FText::FromString(DecreaseKeyText));
	IncreaseInputText->SetText(FText::FromString(IncreaseKeyText));
}

void UTwoMinWidget_CustomProgressbar::SetFocusMuteButton() const
{
	MuteButton->SetFocus();
	SetEnableInputText(false);
}

void UTwoMinWidget_CustomProgressbar::SetEnableInputText(bool bOn) const
{
	DecreaseInputText->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	IncreaseInputText->SetVisibility(bOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UTwoMinWidget_CustomProgressbar::SetCheckMuteButton(bool bOn) const
{
	MuteButton->SetCheckBox(bOn);
}

void UTwoMinWidget_CustomProgressbar::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	HighlightFocusSlot(true);
}

void UTwoMinWidget_CustomProgressbar::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		SetFocus();
		return;
	}
	
	HighlightFocusSlot(false);
}

void UTwoMinWidget_CustomProgressbar::HighlightFocusSlot(bool bOn) const
{
	SetEnableInputText(bOn);
	MuteButton->SetLocked(!bOn);
	
	FLinearColor tColor = bOn ? FLinearColor(1, 1, 1, 1) : FLinearColor(0.2f, 0.2f, 0.2f, 1.f);
	ProgressPointImage->SetColorAndOpacity(tColor);
	DisableImage->SetVisibility(bOn ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}
