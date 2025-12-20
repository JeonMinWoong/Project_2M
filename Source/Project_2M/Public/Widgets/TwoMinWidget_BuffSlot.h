// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/TwoMinWidgetBase.h"
#include "TwoMinWidget_BuffSlot.generated.h"

class UTwoMinWidget_BuffWindow;
struct FItemInstance;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_2M_API UTwoMinWidget_BuffSlot : public UTwoMinWidgetBase
{
	GENERATED_BODY()
	
public:
	void Init(UTwoMinWidget_BuffWindow* NewBuffWindow, int32 InSlotIndex);
	void OnBuff(int32 ItemID);
	void OffBuff();
	void OnCopyBuff(int32 ItemID, float RunningTime);
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* BuffIconMaterialInstance;
	
	UPROPERTY()
	UMaterialInstanceDynamic* CachedMaterialInstanceDynamic;
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	UTwoMinWidget_BuffWindow* BuffWindow;
	
	UPROPERTY()
	int32 BuffItemID;
	
	UPROPERTY(meta=(BindWidget))
	UImage* BuffIconImage;
	
	UPROPERTY()
	float BuffDuration;
	
	UPROPERTY()
	float CurrentTime;
	
	UPROPERTY()
	int32 SlotIndex;
	
	bool bIsOnBuff;
	
public:
	FORCEINLINE int32 GetBuffItemID() const { return BuffItemID; }
	FORCEINLINE float GetRunningTime() const { return CurrentTime; }
};
