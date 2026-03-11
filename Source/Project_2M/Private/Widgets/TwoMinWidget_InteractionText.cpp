// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TwoMinWidget_InteractionText.h"

#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/UI/RichWidgetDecorator.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "System/InteractionActor_NPC.h"
#include "Widgets/TwoMinWidget_KeyType.h"

void UTwoMinWidget_InteractionText::SetInteractionText(const FString& NewInteractionText)
{
	if (!TextBlock) return;
	
	SetFocus();
	bIsInteractionTexting = true;
	
	FString Str = FString::Printf(TEXT("<InteractText>%s</> <widget id=\"NextInteraction\"/>"), *NewInteractionText);
	TextBlock->SetText(FText::FromString(Str));
	
	URichWidgetDecorator* Deco = Cast<URichWidgetDecorator>(TextBlock->GetDecoratorByClass(URichWidgetDecorator::StaticClass()));
	if (Deco)
	{
		Deco->OnWidgetCreated.AddUniqueDynamic(this, &UTwoMinWidget_InteractionText::KeyTypeWidgetCreated);
	}
}

FReply UTwoMinWidget_InteractionText::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey InKey = InKeyEvent.GetKey();
	if (InKey == EKeys::Enter || InKey == EKeys::F || InKey == EKeys::Gamepad_FaceButton_Bottom)
	{
		if (bIsInteractionTexting) return FReply::Unhandled();
		
		ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(GetOwningPlayerPawn());
		if (!PlayerCharacter) return FReply::Unhandled();
		if (!PlayerCharacter->GetInteractionActor()) return FReply::Unhandled();
		
		AInteractionActor_NPC* NPC = Cast<AInteractionActor_NPC>(PlayerCharacter->GetInteractionActor());
		if (!NPC) return FReply::Unhandled();

		NPC->Interact(PlayerCharacter);
		return FReply::Handled();
	}
	
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

void UTwoMinWidget_InteractionText::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	
	if (InFocusEvent.GetCause() == EFocusCause::Navigation)
	{
		SetFocus();
		return;
	}
	
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		SetFocus();
		return;
	}
}

void UTwoMinWidget_InteractionText::KeyTypeWidgetCreated(const FString& WidgetID, UUserWidget* Widget)
{
	UTwoMinWidget_KeyType* KeyTypeWidget = Cast<UTwoMinWidget_KeyType>(Widget);
	if (!KeyTypeWidget) return;
	
	const bool bIsUsingGamePad = UTwoMinFunctionLibrary::IsUsingGamePad(GetWorld(), GetOwningPlayer()->GetPlatformUserId());
	const FString KeyText = bIsUsingGamePad ? TEXT("A") : TEXT("F");
	KeyTypeWidget->SetKeyTextValue(KeyText);
}
