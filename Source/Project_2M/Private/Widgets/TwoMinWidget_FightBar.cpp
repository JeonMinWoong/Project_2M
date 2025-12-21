// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_FightBar.h"

#include "TwoMinFunctionLibrary.h"
#include "Components/ProgressBar.h"
#include "Components/UniformGridPanel.h"
#include "Widgets/TwoMinWidget_FightProgressBar.h"

void UTwoMinWidget_FightBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	TArray<UWidget*> ChildWidgets = GridPanel->GetAllChildren();
	if (ChildWidgets.IsEmpty()) return;

	for (auto ChildWidget : ChildWidgets)
	{
		if (UTwoMinWidget_FightProgressBar* ProgressBar = Cast<UTwoMinWidget_FightProgressBar>(ChildWidget))
		{
			ProgressBar->Init();
			ProgressBars.Add(ProgressBar);
		}
	}
	
	MaxFightBarCount = ProgressBars.Num();
}

void UTwoMinWidget_FightBar::SetPercent(float NewPercent)
{
	const float TotalPercent = NewPercent * MaxFightBarCount;
	const int32 IndexCount = FMath::FloorToInt32(TotalPercent);
	const float RemainPercent = TotalPercent - IndexCount;
	
	bool bIsFull = UTwoMinFunctionLibrary::IsNearFloatEqual(TotalPercent, MaxFightBarCount);
	
	for (int32 Index = IndexCount - 1; Index < MaxFightBarCount; ++Index)
	{
		if (Index < 0)
		{
			Index = 0;
		}
		
		ProgressBars[Index]->SetPercent(0);
		ProgressBars[Index]->SetColor(false, bIsOnAngerMode);
	}
	
	for (int32 Index = 0; Index < IndexCount; Index++)
	{
		ProgressBars[Index]->SetPercent(1);
		ProgressBars[Index]->SetColor(bIsFull, bIsOnAngerMode);
	}
	
	if (UTwoMinFunctionLibrary::IsNearFloatZero(RemainPercent)) return;
	
	ProgressBars[IndexCount]->SetPercent(RemainPercent);
	ProgressBars[IndexCount]->SetColor(bIsFull, bIsOnAngerMode);
}

void UTwoMinWidget_FightBar::SetOnAngerMode(bool NewOnAngerMode)
{
	bIsOnAngerMode = NewOnAngerMode;

	for (const auto ProgressBar : ProgressBars)
	{
		ProgressBar->SetColor(false, bIsOnAngerMode);
	}
}
