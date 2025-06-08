// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/CharacterDataAsset.h"

UInputAction* UCharacterDataAsset::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FCharacterInputConfig& InputActionConfig : NativeInputActions)
	{
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			return InputActionConfig.InputAction;
		}
	}

	return nullptr;
}
