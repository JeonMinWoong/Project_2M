// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_GoldNotify.h"

#include "Components/TextBlock.h"

void UTwoMinWidget_GoldNotify::SetGoldNotifyText(const FText& GoldText) const
{
	GoldNotifyText->SetText(GoldText);
}
