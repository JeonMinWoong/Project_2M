// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_InputKey_Notify.h"

#include "Widgets/TwoMinWidget_KeyType.h"

void UTwoMinWidget_InputKey_Notify::OnEnable(bool bOn)
{
	if (!bOn) return;

	const FString KeyText = IsUsingGamePad() ? TEXT("A") : TEXT("F");
	KeyType->SetKeyTextValue(KeyText);
}

void UTwoMinWidget_InputKey_Notify::OnInputDeviceChanged(bool bIsGamePad)
{
	const FString KeyText = bIsGamePad ? TEXT("A") : TEXT("F");
	KeyType->SetKeyTextValue(KeyText);
}
