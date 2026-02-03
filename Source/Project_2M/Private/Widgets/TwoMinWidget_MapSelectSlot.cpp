// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_MapSelectSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTwoMinWidget_MapSelectSlot::InitSlot()
{
	BaseImage->SetBrushFromTexture(NormalTexture, false);
	MapNameTextBlock->SetText(MapName);
}

void UTwoMinWidget_MapSelectSlot::SetLocked(bool bOn)
{
	bIsLocked = bOn;
	if (bIsLocked)
	{
		BaseImage->SetBrushFromTexture(LockTexture, false);
	}
	else
	{
		BaseImage->SetBrushFromTexture(NormalTexture, false);
	}
}

void UTwoMinWidget_MapSelectSlot::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	FocusMapSelectSlot(true);
}

void UTwoMinWidget_MapSelectSlot::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (InFocusEvent.GetCause() == EFocusCause::Navigation)
	{
		SetFocus();
		return;
	}
	
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		SetFocus();
		return;
	}
	
	FocusMapSelectSlot(false);
}

void UTwoMinWidget_MapSelectSlot::FocusMapSelectSlot(const bool bOn)
{
	if (bIsLocked) return;
	
	if (bOn)
	{
		BaseImage->SetBrushFromTexture(FocusTexture, false);	
	}
	else
	{
		BaseImage->SetBrushFromTexture(NormalTexture, false);
	}
}
