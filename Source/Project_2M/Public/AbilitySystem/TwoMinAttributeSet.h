// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "TwoMinAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class ATwoMinPlayerCharacter;
class IBaseUIInterface;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UTwoMinAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Level")
	FGameplayAttributeData MaxLevel;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, MaxLevel)

	UPROPERTY(BlueprintReadOnly, Category = "Level")
	FGameplayAttributeData CurrentLevel;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, CurrentLevel)
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, MaxHealth)
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, MaxStamina)
	
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, CurrentStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Experience")
	FGameplayAttributeData MaxExperience;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, MaxExperience)

	UPROPERTY(BlueprintReadOnly, Category = "Experience")
	FGameplayAttributeData CurrentExperience;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, CurrentExperience)
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTo;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, DamageTo)

	UPROPERTY(BlueprintReadOnly, Category = "GiveExperience")
	FGameplayAttributeData GiveExperience;
	ATTRIBUTE_ACCESSORS(UTwoMinAttributeSet, GiveExperience)

private:
	int32 NeedToExperienceValue(ATwoMinPlayerCharacter* PlayerCharacter, int32 InCurrentLevel) const;
	
	TWeakInterfacePtr<IBaseUIInterface> CachedBaseUInterface;
};
