// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "RichWidgetDecorator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRichWidgetCreated, const FString&, WidgetID, UUserWidget*, Widget);

/**
 * 
 */
UCLASS()
class PROJECT_2M_API URichWidgetDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()
	
public:
	URichWidgetDecorator(const FObjectInitializer& ObjectInitializer);

	// 핵심: 태그를 만났을 때 어떤 Slate 위젯을 생성할지 결정
	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	
	UPROPERTY(BlueprintAssignable, Category = "RichText|Event")
	FOnRichWidgetCreated OnWidgetCreated;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	UDataTable* WidgetSetTable;
	
public:
	FORCEINLINE UDataTable* GetWidgetSetTable() const { return WidgetSetTable; }
};
