
#include "Item/PickUp/TwoMinPickUpItemBase.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/InventoryComponent.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Managers/ItemDataManager.h"

ATwoMinPickUpItemBase::ATwoMinPickUpItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->InitSphereRadius(PickUpRange);
	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnEndOverlap);
}

void ATwoMinPickUpItemBase::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->SetSphereRadius(PickUpRange);
}

void ATwoMinPickUpItemBase::MakeItemDropBox(TPair<int32, int32> Item)
{
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	EItemType ItemType = UTwoMinFunctionLibrary::GetItemType(Item.Key);
	if (ItemType == EItemType::Equipment)
	{
		FItemEquipmentData NewEquipmentData = GI->ItemDataManager->GetItemEquipmentData(Item.Key);
		
		FItemEquipmentData ItemEquipmentData;
		ItemEquipmentData.ItemDataBase = NewEquipmentData.ItemDataBase;
		ItemEquipmentData.ItemDataBase.CurrentCount = Item.Value;
		if (NewEquipmentData.EquipmentPower.IsEmpty() == false)
		{
			for (auto Power : NewEquipmentData.EquipmentPower)
			{
				ItemEquipmentData.EquipmentPower.Add(Power);	
			}	
		}

		ItemEquipmentList.Add(ItemEquipmentData);
	}
	else if (ItemType == EItemType::Consume)
	{
		FItemConsumeData NewConsumeData = GI->ItemDataManager->GetItemConsumeData(Item.Key);
		
		FItemConsumeData ItemConsumeData;
		ItemConsumeData.ItemDataBase = NewConsumeData.ItemDataBase;
		ItemConsumeData.ItemDataBase.CurrentCount = Item.Value;
		if (NewConsumeData.ConsumePower.IsEmpty() == false)
		{
			for (auto Power : NewConsumeData.ConsumePower)
			{
				ItemConsumeData.ConsumePower.Add(Power);	
			}
		}
		
		ItemConsumeList.Add(ItemConsumeData);
	}
	else if (ItemType == EItemType::Etc)
	{
		FItemEtcData NewEtcData = GI->ItemDataManager->GetItemEtcData(Item.Key);
		
		FItemEtcData ItemEtcData;
		ItemEtcData.ItemDataBase = NewEtcData.ItemDataBase;
		ItemEtcData.ItemDataBase.CurrentCount = Item.Value;

		ItemEtcList.Add(ItemEtcData);
	}
}

void ATwoMinPickUpItemBase::GetUpItem(const ATwoMinPlayerCharacter* PlayerCharacter)
{
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	UInventoryComponent* Inventory = PlayerCharacter->GetInventoryComponent();
	int32 SaveAllItemCount = 0;
	for (const FItemEquipmentData& EquipmentList : ItemEquipmentList)
	{
		FItemEquipmentData NewEquipmentData = GI->ItemDataManager->GetItemEquipmentData(EquipmentList.ItemDataBase.ItemID);
		Inventory->SaveToEquipmentInventory(EquipmentList, NewEquipmentData.ItemDataBase.ItemName);	
		SaveAllItemCount++;
	}

	ItemEquipmentList.Empty();
	
	for (const FItemConsumeData& ConsumeList : ItemConsumeList)
	{
		FItemConsumeData NewConsumeData = GI->ItemDataManager->GetItemConsumeData(ConsumeList.ItemDataBase.ItemID);
		Inventory->SaveToConsumeInventory(ConsumeList, NewConsumeData.ItemDataBase.ItemName);
		SaveAllItemCount++;
	}

	ItemConsumeList.Empty();
	
	for (const FItemEtcData& EtcList : ItemEtcList)
	{
		FItemEtcData NewEtcData = GI->ItemDataManager->GetItemEtcData(EtcList.ItemDataBase.ItemID);
		Inventory->SaveToEtcInventory(EtcList, NewEtcData.ItemDataBase.ItemName);
		SaveAllItemCount++;
	}

	Inventory->UpdateInventory();
	Inventory->ShowPickUpGetItem(SaveAllItemCount);
	ItemEtcList.Empty();
	
	Destroy();
}

void ATwoMinPickUpItemBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATwoMinPlayerCharacter* OverLappedHeroCharacter = Cast<ATwoMinPlayerCharacter>(OtherActor))
	{
		StartItemPickUpAbility(OverLappedHeroCharacter);
	}
}

void ATwoMinPickUpItemBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsTryPickUpAbility = false;
}

void ATwoMinPickUpItemBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsTryPickUpAbility)
	{
		TArray<AActor*> PlayerActor;
		const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		const TArray<AActor*> IgnoreActors;
		
		UKismetSystemLibrary::SphereOverlapActors(
			GetWorld(),
			GetActorLocation(),
			PickUpRange,
			ObjectTypes,
			ATwoMinPlayerCharacter::StaticClass(),
			IgnoreActors,
			PlayerActor
		);

		for (AActor* Actor : PlayerActor)
		{
			if (ATwoMinPlayerCharacter* PlayerCharacter = Cast<ATwoMinPlayerCharacter>(Actor))
			{
				if (PlayerCharacter->GetAbilitySystemComponent()->
					IsPlayingAbility(TwoMinGameplayTag::Player_Ability_ItemPickUp) == false)
				{
					StartItemPickUpAbility(PlayerCharacter);	
				}
			}
		}
	}
}

void ATwoMinPickUpItemBase::StartItemPickUpAbility(const ATwoMinPlayerCharacter* PlayerCharacter)
{
	bIsTryPickUpAbility = PlayerCharacter->GetAbilitySystemComponent()->
	TryActivateAbilityByTag(TwoMinGameplayTag::Player_Ability_ItemPickUp);
}
