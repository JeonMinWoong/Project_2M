// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_GoldInfo.h"

#include "TwoMinFunctionLibrary.h"
#include "Components/TextBlock.h"
#include "Widgets/TwoMinWidget_GoldNotify.h"

void UTwoMinWidget_GoldInfo::SetGoldText(const int32 InGoldAmount)
{
	FText GoldText = FText::AsNumber(InGoldAmount);
	GoldTextBlock->SetText(GoldText);
}

void UTwoMinWidget_GoldInfo::SetGoldNotifyText(int32 GainAmount)
{
	if (UTwoMinFunctionLibrary::IsLoadingData(this)) return;
	if (GainAmount <= 0) return;
	
	GoldNotify->SetVisibility(ESlateVisibility::Visible);
	const FString Str = FString::Printf(TEXT("+ %d"), GainAmount);
	const FText GoldText = FText::FromString(Str);
	GoldNotify->SetGoldNotifyText(GoldText);
	
	if (GetAnimationCurrentTime(WAnim_GainGold) > 0.f)
	{
		StopAnimation(WAnim_GainGold);	
	}
	
	PlayAnimation(WAnim_GainGold);
}

void UTwoMinWidget_GoldInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GoldNotify->SetVisibility(ESlateVisibility::Hidden);
}
