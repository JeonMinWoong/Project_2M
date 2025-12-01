// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_ItemPickUpSlot.h"

#include "Components/EditableTextBox.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/WidgetAnimationDelegateBinding.h"
#include "Components/Image.h"

void UTwoMinWidget_ItemPickUpSlot::SetItemSlotInfo(const FString& ItemName, const int32 ItemCount, UTexture2D* ItemTexture)
{
	bIsActive = true;
	FString PickUpStr = FString::Printf(TEXT("%s "), *ItemName);
	if (ItemCount > 0)
	{
		PickUpStr += FString::Printf(TEXT("x %d"), ItemCount);	
	}
	else
	{
		PickUpStr += FString::Printf(TEXT("MAX"));
	}
	
	FText Text = FText::FromString(PickUpStr);
	ItemText->SetText(Text);
	ItemIcon->SetBrushFromTexture(ItemTexture);
	PlayAnimation(OnItemPickUp);
}

void UTwoMinWidget_ItemPickUpSlot::CopyItemSlotData(FText& OutText, float& OutPlayTime, UTexture2D*& OutItemTexture)
{
	OutText = ItemText->GetText();
	if (UTexture2D* CopyTexture =  Cast<UTexture2D>(ItemIcon->GetBrush().GetResourceObject()))
	{
		OutItemTexture = CopyTexture;	
	}
	
	OutPlayTime = GetAnimationCurrentTime(OnItemPickUp);
}

void UTwoMinWidget_ItemPickUpSlot::SetCopyItemSlotInfo(const FText& CopyText, const float CopyPlayTime,
	UTexture2D* CopyTexture)
{
	FString Str = CopyText.ToString();
	ItemText->SetText(CopyText);
	ItemIcon->SetBrushFromTexture(CopyTexture);
	PlayAnimation(OnItemPickUp);
	SetAnimationCurrentTime(OnItemPickUp, CopyPlayTime);
	bIsActive = true;
}

void UTwoMinWidget_ItemPickUpSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (OnItemPickUp)
	{
		AnimationStartEvent.BindDynamic(this, &UTwoMinWidget_ItemPickUpSlot::OnActivateEvent);
		BindToAnimationStarted(OnItemPickUp, AnimationStartEvent);
		
		AnimationEndEvent.BindDynamic(this, &UTwoMinWidget_ItemPickUpSlot::OnDeactivateEvent);
		BindToAnimationFinished(OnItemPickUp, AnimationEndEvent);
	}
}


void UTwoMinWidget_ItemPickUpSlot::StartRemoveProcess(const float PlayTime)
{
	if (IsAnimationPlaying(OnItemPickUp))
	{
		StopAnimation(OnItemPickUp);
	}
	
	PlayAnimation(OnItemPickUp, PlayTime);
}

void UTwoMinWidget_ItemPickUpSlot::OnActivateEvent()
{
	bIsActive = true;
}

void UTwoMinWidget_ItemPickUpSlot::OnDeactivateEvent()
{
	bIsActive = false;
}
