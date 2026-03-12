// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_InputKey_Notify.h"

#include "TwoMinFunctionLibrary.h"
#include "Widgets/TwoMinWidget_KeyType.h"

void UTwoMinWidget_InputKey_Notify::OnEnable(bool bOn)
{
	if (!bOn) return;
	
	const bool bIsUsingGamePad = UTwoMinFunctionLibrary::IsUsingGamePad(GetWorld(), GetOwningPlayer()->GetPlatformUserId());
	const FString KeyText = bIsUsingGamePad ? TEXT("A") : TEXT("F");
	KeyType->SetKeyTextValue(KeyText);
}
