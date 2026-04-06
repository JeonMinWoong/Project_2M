// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidgetBase.h"

#include "TwoMinFunctionLibrary.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "ToMinTypes/TwoMinEnumTypes.h"

void UTwoMinWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const UGameInstance* GI = GetGameInstance())
	{
		if (UInputDeviceSubsystem* InputDeviceSS = GI->GetEngine()->GetEngineSubsystem<UInputDeviceSubsystem>())
		{
			InputDeviceSS->OnInputHardwareDeviceChanged.AddDynamic(this, &UTwoMinWidgetBase::HandleInputDeviceChanged);
		}
	}

	bCachedIsGamePad = IsUsingGamePad();
}

void UTwoMinWidgetBase::NativeDestruct()
{
	if (const UGameInstance* GI = GetGameInstance())
	{
		if (UInputDeviceSubsystem* InputDeviceSS = GI->GetEngine()->GetEngineSubsystem<UInputDeviceSubsystem>())
		{
			InputDeviceSS->OnInputHardwareDeviceChanged.RemoveDynamic(this, &UTwoMinWidgetBase::HandleInputDeviceChanged);
		}
	}

	Super::NativeDestruct();
}

bool UTwoMinWidgetBase::IsUsingGamePad() const
{
	const APlayerController* PC = GetOwningPlayer();
	if (!PC) return false;

	return UTwoMinFunctionLibrary::IsUsingGamePad(GetWorld(), PC->GetPlatformUserId());
}

void UTwoMinWidgetBase::HandleInputDeviceChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId)
{
	const bool bNewIsGamePad = IsUsingGamePad();
	if (bCachedIsGamePad == bNewIsGamePad) return;

	bCachedIsGamePad = bNewIsGamePad;
	OnInputDeviceChanged(bNewIsGamePad);
}

void UTwoMinWidgetBase::PlayUISound(const EUISoundType NewUISoundType) const
{
	const UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
	if (GI) GI->PlayUISound(NewUISoundType);
}
