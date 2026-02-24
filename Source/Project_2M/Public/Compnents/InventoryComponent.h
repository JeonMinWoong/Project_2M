
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "InventoryComponent.generated.h"


class UTwoMinWidget_InventorySlot;
class UTwoMinWidget_InventoryUI;
class UTwoMinWidgetBase;
struct FItemInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_2M_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	
	void InitGiveItem();
	void OpenInventory(const bool bIsOpenInventory);
	
	void SaveToEquipmentInventory(const FItemEquipmentData& EquipmentData, const FString& ItemName, bool bIsClearStage);
	void SaveToConsumeInventory(const FItemConsumeData& ConsumeData, const FString& ItemName, bool bIsClearStage);
	void SaveToEtcInventory(const FItemEtcData& EtcData, const FString& ItemName, bool bIsClearStage);
	int32 SaveToFinalInventory(const int32 ItemID, const int32 ItemCount, const int32 ItemMaxCount, UTexture2D* ItemTexture);
	void SaveItemPickUpSlotData(const int32 ItemId, const FString& ItemName, int32 ItemCount, UTexture2D* ItemTexture);
	void ShowPickUpGetItem(int32 SaveAllItemCount);
	void UpdateInventory();
	void UseItem(int32 ItemID, bool& bIsRemoved);
	FItemInstance* FindItemInstance(int32 ItemID);
	void ForceEquipmentItem(int32 ItemID, EEquipmentType EquipmentType);

	FItemInstance GetQuickSlotItemInstance(int32 SlotIndex) const;
	void UpdateStatus();
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TArray<FItemInstance> Inventory;
	
	UPROPERTY()
	TArray<FItemPickUpEntry> ItemPickUpSlotData;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTwoMinWidget_InventoryUI> InventoryUIClass;
	
	UPROPERTY()
	UTwoMinWidget_InventoryUI* InventoryUI;
	
public:
	FORCEINLINE UTwoMinWidget_InventoryUI* GetInventoryUI() const { return InventoryUI; }
	FORCEINLINE TArray<FItemInstance> GetInventory() const { return Inventory; }
	FORCEINLINE void SetInventory(const TArray<FItemInstance>& LoadItems) { Inventory = LoadItems; }
};
