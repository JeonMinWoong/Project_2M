// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToMinTypes/TwoMinEnumTypes.h"
#include "Widgets/TwoMinCharacterWidgetBase.h"
#include "Widgets/TwoMinWidget_BuffWindow.h"
#include "TwoMinWidgetPlayer.generated.h"

class UTwoMinWidget_InputKey_Notify;
class UTwoMinWidget_InteractionChoice;
class UTwoMinWidget_InteractionText;
class UCanvasPanel;
class UTwoMinWidget_FightBar;
class UTwoMinWidget_BuffWindow;
struct FItemInstance;
class UTwoMinWidget_WindowQuickSlot;
class UTwoMinWidget_GoldInfo;
struct FItemPickUpEntry;
class UTwoMinWidget_ItemPickUpWindow;
class UTextBlock;
class UProgressBar;
class UPlayerUIComponent;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidgetPlayer : public UTwoMinCharacterWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void InitPlayerUIComponent(UPlayerUIComponent* HeroUIComponent);
	
	virtual void SetCurrentHealthPercent(float Percent) override;

	UFUNCTION()
	void SetCurrentStaminaPercent(float Percent);

	UFUNCTION()
	void SetCurrentFightPercent(float Percent);
	
	UFUNCTION()
	void SetAngerState(bool bIsOnAngerMode);
	
	UFUNCTION()
	void SetCurrentExperiencePercent(float Percent);

	UFUNCTION()
	void SetCurrentLevelValue(int32 InCurrentLevel);

	UFUNCTION()
	void SetPossiblePickUpItem(bool bIsPossiblePickUp);
	
	UFUNCTION()
	void SetItemPickUpWindow(int32 ShowAllItem, TArray<FItemPickUpEntry>& ItemList);
	
	UFUNCTION()
	void SetCurrentGoldValue(int32 NewGold, int32 GainGold);
	
	UFUNCTION()
	void OnSetWindowQuickSlot(FItemInstance QuickSlotRegisterItem, int32 SlotIndex, bool bIsRegister);
	
	UFUNCTION()
	void OnSetBuffItem(int32 ItemID);
	
	UFUNCTION()
	void SetPossibleInteraction(EInteractionType NewInteractionType, bool bIsPossibleInteraction);
	
	UFUNCTION()
	void SetInteractionText(const FString& NewInteractionText, const bool bOn);
	
	UFUNCTION()
	bool IsInteractionTexting();
	
	UFUNCTION()
	void SetInteractionChoice(ENPCType InNPCType, bool bOn);
	
	UFUNCTION()
	void OnStartManualSave();
	
private:
	void StartInteractionText();
	
	UFUNCTION()
	void OnFinishInteractionText();
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_FightBar* FightBar;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ExperienceBar;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelText;

	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_InputKey_Notify* PickUpItem;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_ItemPickUpWindow* ItemPickUpWindow;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_GoldInfo* GoldInfo;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_WindowQuickSlot* WindowQuickSlot;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_BuffWindow* BuffWindow;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_InputKey_Notify* Interaction;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_InputKey_Notify* InteractionNPC;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_InteractionText* InteractionText;
	
	UPROPERTY(meta=(BindWidget))
	UTwoMinWidget_InteractionChoice* InteractionChoice;
	
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* OnStartInteractionTextAnim;
	
	UPROPERTY()
	FWidgetAnimationDynamicEvent CompleteStartInteractionTextAnimEvent;
	
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* OnStartManualSaveAnim;
	
public:
	FORCEINLINE UTwoMinWidget_WindowQuickSlot* GetWindowQuickSlot() const { return WindowQuickSlot; }
	FORCEINLINE int32 GetMaxBuffCount() const { return BuffWindow->GetMaxSlotIndex(); }
};
