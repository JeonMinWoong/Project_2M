// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_FightProgressBar.h"

void UTwoMinWidget_FightProgressBar::Init()
{
	FightProgressBar->SetPercent(0);
}

void UTwoMinWidget_FightProgressBar::SetPercent(float InPercent)
{
	FightProgressBar->SetPercent(InPercent);
}

void UTwoMinWidget_FightProgressBar::SetColor(const bool bIsFull, const bool bIsOnAngerMode)
{
	if (bIsOnAngerMode)
	{
		FightProgressBar->SetFillColorAndOpacity(FLinearColor::Gray);
		return;	
	}
	
	if (bIsFull)
	{
		FightProgressBar->SetFillColorAndOpacity(FLinearColor::Red);
	}
	else
	{
		FightProgressBar->SetFillColorAndOpacity(FLinearColor(1.0f, 0.468356f, 0.0f, 1.0f));
	}
}
