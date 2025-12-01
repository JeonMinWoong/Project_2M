
#include "Item/PickUp/TwoMinPickUpItemBase.h"

#include "TwoMinDebugHelper.h"
#include "TwoMinGameplayTag.h"
#include "AbilitySystem/TwoMinAbilitySystemComponent.h"
#include "Character/TwoMinPlayerCharacter.h"
#include "Compnents/InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
	EItemType ItemType = GetItemType(Item.Key);
	if (ItemType == EItemType::Equipment)
	{
		FItemEquipmentData NewEquipmentData = GetItemEquipmentData(Item.Key);
		
		FItemEquipmentData ItemEquipmentData;
		ItemEquipmentData.ItemDataBase = NewEquipmentData.ItemDataBase;
		ItemEquipmentData.ItemDataBase.CurrentCount = Item.Value;
		ItemEquipmentData.AttackPower = NewEquipmentData.AttackPower;
		ItemEquipmentData.DefensePower = NewEquipmentData.DefensePower;

		ItemEquipmentList.Add(ItemEquipmentData);
	}
	else if (ItemType == EItemType::Consume)
	{
		FItemConsumeData NewConsumeData = GetItemConsumeData(Item.Key);
		
		FItemConsumeData ItemConsumeData;
		ItemConsumeData.ItemDataBase = NewConsumeData.ItemDataBase;
		ItemConsumeData.ItemDataBase.CurrentCount = Item.Value;
		ItemConsumeData.ConsumePower = NewConsumeData.ConsumePower;

		ItemConsumeList.Add(ItemConsumeData);
	}
	else if (ItemType == EItemType::Etc)
	{
		FItemEtcData NewEtcData = GetItemEtcData(Item.Key);
		
		FItemEtcData ItemEtcData;
		ItemEtcData.ItemDataBase = NewEtcData.ItemDataBase;
		ItemEtcData.ItemDataBase.CurrentCount = Item.Value;

		ItemEtcList.Add(ItemEtcData);
	}
}

EItemType ATwoMinPickUpItemBase::GetItemType(int32 ItemID)
{
	int32 Value = ItemID;

	while (Value >= 10)
	{
		Value /= 10;
	}
	
	if (Value < static_cast<int8>(EItemType::Consume)) return EItemType::Equipment;
	if (Value < static_cast<int8>(EItemType::Etc)) return EItemType::Consume;
	if (Value < static_cast<int8>(EItemType::Unknown)) return EItemType::Etc;

	return EItemType::None;
}

FItemEquipmentData ATwoMinPickUpItemBase::GetItemEquipmentData(int32 ItemID) const
{
	FItemEquipmentData NewEquipmentData;
	TArray<FItemEquipmentData*> EquipmentTableGroup;
	FString DropTableGroupName = FString::FromInt(ItemID);
	EquipmentDataTable->GetAllRows(DropTableGroupName, EquipmentTableGroup);

	for (FItemEquipmentData* TableGroup : EquipmentTableGroup)
	{
		if (ItemID == TableGroup->ItemDataBase.ItemID)
		{
			NewEquipmentData = *TableGroup;
			break;
		}
	}
	
	return NewEquipmentData;
}

FItemConsumeData ATwoMinPickUpItemBase::GetItemConsumeData(int32 ItemID) const
{
	FItemConsumeData NewConsumeData;
	TArray<FItemConsumeData*> ConsumeTableGroup;
	FString DropTableGroupName = FString::FromInt(ItemID);
	ConsumeDataTable->GetAllRows(DropTableGroupName, ConsumeTableGroup);

	for (FItemConsumeData* TableGroup : ConsumeTableGroup)
	{
		if (ItemID == TableGroup->ItemDataBase.ItemID)
		{
			NewConsumeData = *TableGroup;
			break;
		}
	}
	
	return NewConsumeData;
}

FItemEtcData ATwoMinPickUpItemBase::GetItemEtcData(int32 ItemID) const
{
	FItemEtcData NewEtcData;
	TArray<FItemEtcData*> EtcTableGroup;
	FString DropTableGroupName = FString::FromInt(ItemID);
	EtcDataTable->GetAllRows(DropTableGroupName, EtcTableGroup);

	for (FItemEtcData* TableGroup : EtcTableGroup)
	{
		if (ItemID == TableGroup->ItemDataBase.ItemID)
		{
			NewEtcData = *TableGroup;
			break;
		}
	}
	
	return NewEtcData;
}


void ATwoMinPickUpItemBase::GetUpItem(const ATwoMinPlayerCharacter* PlayerCharacter)
{
	UInventoryComponent* Inventory = PlayerCharacter->GetInventoryComponent();
	int32 SaveAllItemCount = 0;
	for (const FItemEquipmentData& EquipmentList : ItemEquipmentList)
	{
		FItemEquipmentData NewEquipmentData = GetItemEquipmentData(EquipmentList.ItemDataBase.ItemID);
		Inventory->SaveToEquipmentInventory(EquipmentList, NewEquipmentData.ItemDataBase.ItemName);	
		SaveAllItemCount++;
	}

	ItemEquipmentList.Empty();
	
	for (const FItemConsumeData& ConsumeList : ItemConsumeList)
	{
		FItemConsumeData NewConsumeData = GetItemConsumeData(ConsumeList.ItemDataBase.ItemID);
		Inventory->SaveToConsumeInventory(ConsumeList, NewConsumeData.ItemDataBase.ItemName);
		SaveAllItemCount++;
	}

	ItemConsumeList.Empty();
	
	for (const FItemEtcData& EtcList : ItemEtcList)
	{
		FItemEtcData NewEtcData = GetItemEtcData(EtcList.ItemDataBase.ItemID);
		Inventory->SaveToEtcInventory(EtcList, NewEtcData.ItemDataBase.ItemName);
		SaveAllItemCount++;
	}

	Inventory->ShowAllItem(SaveAllItemCount);
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
