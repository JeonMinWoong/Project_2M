
#include "Compnents/ItemDropComponent.h"

UItemDropComponent::UItemDropComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

TMap<int32, int32> UItemDropComponent::TryGetDropItems() const
{
	TMap<int32, int32> DropItems;
	TArray<FItemDropData*> DropTableGroup;
	
	if (DropTableOwner.IsEmpty())
	{
		return DropItems;
	}
	
	DropTable->GetAllRows(DropTableOwner, DropTableGroup);

	for (FItemDropData* TableItem : DropTableGroup)
	{
		if (TableItem && TableItem->ItemDropOwner == DropTableOwner)
		{
			int32 ItemCode = 0;
			int32 DropCount = 0;
			CalculateDropProbability(TableItem, ItemCode, DropCount);
			if (ItemCode == 0 || DropCount == 0) continue;
			if (DropItems.Contains(ItemCode)) continue;
			
			DropItems.Add(ItemCode, DropCount);
		}
	}
	
	return DropItems;
}

void UItemDropComponent::CalculateDropProbability(const FItemDropData* ItemDropData, int32& OutItemCode,
	int32& OutDropCount) const
{
	const float RandValue = FMath::RandRange(0.f, 100.f);
	if (RandValue <= ItemDropData->Weight)
	{
		OutItemCode = ItemDropData->ItemID;
		OutDropCount = FMath::RandRange(ItemDropData->MinDropRange, ItemDropData->MaxDropRangeCount);
		return;
	}

	OutItemCode = 0;
	OutDropCount = 0;
}

