// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TwoMinWidgetBase.generated.h"

enum class EUISoundType : uint8;

UCLASS()
class PROJECT_2M_API UTwoMinWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	virtual void OnInputDeviceChanged(bool bIsGamePad) {}

	bool IsUsingGamePad() const;

	void PlayUISound(const EUISoundType NewUISoundType) const;

private:
	UFUNCTION()
	void HandleInputDeviceChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId);

	bool bCachedIsGamePad = false;
};
