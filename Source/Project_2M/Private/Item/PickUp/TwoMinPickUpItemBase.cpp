
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
	if (!GI) return;
	
	GI->ItemDataManager->GetDropItemList(Item, ItemEquipmentList, ItemConsumeList, ItemEtcList);
}

void ATwoMinPickUpItemBase::GetUpItem(const ATwoMinPlayerCharacter* PlayerCharacter)
{
	UTwoMinGameInstance* GI = GetWorld()->GetGameInstance<UTwoMinGameInstance>();
	if (!GI) return;
	
	GI->ItemDataManager->GiveToInventory(PlayerCharacter, ItemEquipmentList, ItemConsumeList, ItemEtcList, false);
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
