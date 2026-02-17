
#include "Compnents/ItemDropComponent.h"

#include "GameInstance/TwoMinGameInstance.h"
#include "Managers/ItemDataManager.h"

UItemDropComponent::UItemDropComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

TMap<int32, int32> UItemDropComponent::TryGetCharacterDropItems() const
{
	TMap<int32, int32> DropItems;
	
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetWorld()->GetGameInstance());
	if (!GI) return DropItems;
	
	DropItems = GI->ItemDataManager->TryGetDropItems(DropTableOwner);
	return DropItems;
}
