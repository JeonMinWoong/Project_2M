// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_LoadingUI.h"

#include "Components/TextBlock.h"

void UTwoMinWidget_LoadingUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	LoadingCount = -1;
	GetWorld()->GetTimerManager().SetTimer(LoadingTimerHandle, 
		this, &UTwoMinWidget_LoadingUI::UpdateLoadingText, UpdateLoadingTime, true);
}

void UTwoMinWidget_LoadingUI::NativeDestruct()
{
	Super::NativeDestruct();
	
	GetWorld()->GetTimerManager().ClearTimer(LoadingTimerHandle);
}

void UTwoMinWidget_LoadingUI::UpdateLoadingText()
{
	LoadingCount = (LoadingCount + 1) % 4;
	
	FString Str = FString::ChrN(LoadingCount, '.');
	LoadingText->SetText(FText::FromString(TEXT("Loading") + Str));
}
