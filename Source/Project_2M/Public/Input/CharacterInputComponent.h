// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "DataAssets/CharacterDataAsset.h"
#include "CharacterInputComponent.generated.h"

class UCharacterDataAsset;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UCharacterInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UCharacterDataAsset* InInputConfig, const FGameplayTag InInputTag,
		ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);
};

template<class UserObject, typename CallbackFunc>
inline void UCharacterInputComponent::BindNativeInputAction(const UCharacterDataAsset* InInputConfig,
	const FGameplayTag InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}
