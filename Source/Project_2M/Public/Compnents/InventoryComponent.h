
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToMinTypes/TwoMinStructTypes.h"
#include "InventoryComponent.generated.h"


struct FItemInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_2M_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	void SaveToEquipmentInventory(const FItemEquipmentData& EquipmentData, const FString& ItemName);
	void SaveToConsumeInventory(const FItemConsumeData& ConsumeData, const FString& ItemName);
	void SaveToEtcInventory(const FItemEtcData& EtcData, const FString& ItemName);
	int32 SaveToFinalInventory(const int32 ItemID, const int32 ItemCount, const int32 ItemMaxCount);
	void SaveItemPickUpSlotData(const FString& ItemName, int32 ItemCount, UTexture2D* ItemTexture);
	void ShowAllItem(int32 SaveAllItemCount);

private:
	UPROPERTY(VisibleAnywhere)
	TArray<FItemInstance> Inventory;
	
	UPROPERTY()
	TArray<FItemPickUpEntry> ItemPickUpSlotData;
};
