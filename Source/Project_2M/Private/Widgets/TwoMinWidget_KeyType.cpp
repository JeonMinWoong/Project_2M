// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_KeyType.h"

#include "Components/TextBlock.h"

void UTwoMinWidget_KeyType::SetKeyTextValue(const FString& NewKeyTypeText) const
{
	KeyTypeTextBlock->SetText(FText::FromString(NewKeyTypeText));
}
