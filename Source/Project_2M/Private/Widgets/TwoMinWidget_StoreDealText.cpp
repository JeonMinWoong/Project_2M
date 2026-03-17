// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_StoreDealText.h"

#include "Components/TextBlock.h"

void UTwoMinWidget_StoreDealText::SetTextValue(const FString& DealSuccessText)
{
	if (!TextBlock) return;
	if (DealSuccessText.IsEmpty()) return;
	
	StartFadeOutAnim();
	SetVisibility(ESlateVisibility::Visible);
	TextBlock->SetText(FText::FromString(DealSuccessText));
}

void UTwoMinWidget_StoreDealText::StartFadeOutAnim()
{
	if (!OnStartFadeOutAnim) return;
	
	if (IsAnimationPlaying(OnStartFadeOutAnim))
	{
		StopAnimation(OnStartFadeOutAnim);
	}
	
	PlayAnimation(OnStartFadeOutAnim);
}
