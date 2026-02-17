// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TwoMinEnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "TwoMinDebugHelper.h"
#include "Blueprint/UserWidget.h"
#include "Compnents/ItemDropComponent.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "Compnents/UI/EnemyUIComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameModes/TwoMinBaseGameMode.h"
#include "Item/PickUp/TwoMinPickUpItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Enemy/TwoMinWidgetEnemy.h"

ATwoMinEnemyCharacter::ATwoMinEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = CharacterRotationRate;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f; // 감속 속도

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");
	ItemDropComponent = CreateDefaultSubobject<UItemDropComponent>("ItemDropComponent");
	
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidgetComponent");
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());

	EnemyExecutionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyExecutionWidgetComponent");
	EnemyExecutionWidgetComponent->SetupAttachment(GetMesh());
	
	CharacterType = ECharacterType::Enemy;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ATwoMinEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!EnemyExecutionWidgetComponent || EnemyExecutionWidgetComponent->IsVisible() == false) return;

	EnemyExecutionWidgetComponent->SetWorldRotation(
		(GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation() - 
		 EnemyExecutionWidgetComponent->GetComponentLocation()).Rotation()
	);
}

UBaseCombatComponent* ATwoMinEnemyCharacter::GetCombatComponent() const
{
	return EnemyCombatComponent;
}

UBaseUIComponent* ATwoMinEnemyCharacter::GetBaseUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* ATwoMinEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void ATwoMinEnemyCharacter::BeforeDeathProcess()
{
	Super::BeforeDeathProcess();

	AAIController* AI = Cast<AAIController>(GetController());
	if (!AI) return;

	AI->StopMovement();
	AI->BrainComponent->StopLogic(TEXT("AI Disabled"));
	AI->UnPossess();
}

void ATwoMinEnemyCharacter::AfterDeathProcess()
{
	Super::AfterDeathProcess();
	
	TMap<int32, int32> DropItems = ItemDropComponent->TryGetCharacterDropItems();
	
	if (DropItems.IsEmpty())
	{
		TwoMinDebugHelper::Print(TEXT("아이템 드랍 없음."));
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	FHitResult HitResult;
	FVector Start = GetActorLocation() + FVector(0.f, 0.f, 100.f);
	FVector End = Start - FVector(0.f, 0.f, 1000.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility, // 충돌 채널
		Params
	);

	if (bHit == false) return;
	
	FVector SpawnLocation = HitResult.ImpactPoint + FVector(0.f, 0.f, 75.f);
	FRotator SpawnRotation = GetActorRotation();

	ATwoMinPickUpItemBase* ItemDropBox = GetWorld()->SpawnActor<ATwoMinPickUpItemBase>(
		ItemDropClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (ItemDropBox)
	{
		for (TPair DropItem : DropItems)
		{
			ItemDropBox->MakeItemDropBox(DropItem);
		}
	}
}

void ATwoMinEnemyCharacter::OnDestroyedProcess()
{
	Super::OnDestroyedProcess();
	
	ClearStageProcess();
}

void ATwoMinEnemyCharacter::ClearStageProcess() const
{
	ATwoMinBaseGameMode* GM = Cast<ATwoMinBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return;
	
	GM->ShowClearStageUI();
}

void ATwoMinEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
		}
	}
}

void ATwoMinEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitEnemyHealthWidget();
}

void ATwoMinEnemyCharacter::InitEnemyHealthWidget()
{
	UUserWidget* UserWidget = EnemyHealthWidgetComponent->GetUserWidgetObject();
	if (!UserWidget) return;
		
	UTwoMinWidgetEnemy* HealthWidget = Cast<UTwoMinWidgetEnemy>(UserWidget);
	if (!HealthWidget) return;
	
	if (bUseBossHealthBar)
	{
		HealthWidget->HideWorldHealthBar();
	}
	else
	{
		HealthWidget->InitEnemyUIComponent(EnemyUIComponent);
	}
}