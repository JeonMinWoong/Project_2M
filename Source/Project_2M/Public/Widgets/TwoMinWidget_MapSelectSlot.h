// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TwoMinWidgetBase.h"
#include "TwoMinWidget_MapSelectSlot.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_MapSelectSlot : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void InitSlot();
	void SetLocked(bool bOn);
	
protected:
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	
	void FocusMapSelectSlot(bool bOn);

private:
	UPROPERTY(meta=(BindWidget))
	UImage* BaseImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MapNameTextBlock;
	
	UPROPERTY(EditAnywhere, Category="MapSelectSlot|MapName")
	FText MapName;
	
	UPROPERTY(EditAnywhere, Category="MapSelectSlot|NormalTexture")
	UTexture2D* NormalTexture;
	
	UPROPERTY(EditAnywhere, Category="MapSelectSlot|FocusTexture")
	UTexture2D* FocusTexture;
	
	UPROPERTY(EditAnywhere, Category="MapSelectSlot|LockTexture")
	UTexture2D* LockTexture;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsLocked = false;
	
public:
	FORCEINLINE bool IsLocked() const { return bIsLocked; }
	
};
