// Fill out your copyright notice in the Description page of Project Settings.


#include "Compnents/UI/RichWidgetDecorator.h"

#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "ToMinTypes/TwoMinStructTypes.h"

class FRichWidgetDecorator : public FRichTextDecorator
{
public:
	FRichWidgetDecorator(URichTextBlock* InOwner, URichWidgetDecorator* InRichWidgetDecorator)
		: FRichTextDecorator(InOwner), Decorator(InRichWidgetDecorator) {}

	// <widget id="ID"/> 형태의 태그인지 확인
	virtual bool Supports(const FTextRunParseResults& RunParseResults, const FString& Text) const override
	{
		return RunParseResults.Name == TEXT("widget") && RunParseResults.MetaData.Contains(TEXT("id"));
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& DefaultTextStyle) const override
	{
		const FString& WidgetID = RunInfo.MetaData[TEXT("id")];
		const FRichWidgetRow* Row = Decorator->GetWidgetSetTable()->FindRow<FRichWidgetRow>(FName(*WidgetID), TEXT(""));
		
		if (!Row || !Row->WidgetClass)
		{
			return nullptr; 
		}

		if (UUserWidget* NewWidget = CreateWidget<UUserWidget>(Owner->GetWorld(), Row->WidgetClass))
		{
			Decorator->CachedWidget = NewWidget;
			Decorator->OnWidgetCreated.Broadcast(WidgetID, NewWidget);
			return NewWidget->TakeWidget();
		}
		
		return nullptr;
	}

private:
	URichWidgetDecorator* Decorator;
};

URichWidgetDecorator::URichWidgetDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

TSharedPtr<ITextDecorator> URichWidgetDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FRichWidgetDecorator(InOwner, this));
}