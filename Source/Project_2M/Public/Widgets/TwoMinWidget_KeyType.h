// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_KeyType.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_KeyType : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetKeyTextValue(const FString& NewKeyTypeText) const;
	
private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* KeyTypeTextBlock;
	
public:
	FORCEINLINE FText GetKeyTextValue() const { return KeyTypeTextBlock->GetText(); };
};
