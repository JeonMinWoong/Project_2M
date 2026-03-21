// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidgetBase.h"

#include "GameInstance/TwoMinGameInstance.h"
#include "ToMinTypes/TwoMinEnumTypes.h"

void UTwoMinWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UTwoMinWidgetBase::PlayUISound(const EUISoundType NewUISoundType) const
{
	const UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (GI) GI->PlayUISound(NewUISoundType);
}
