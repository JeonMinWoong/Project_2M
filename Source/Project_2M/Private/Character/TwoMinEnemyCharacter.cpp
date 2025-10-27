// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TwoMinEnemyCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "Compnents/UI/EnemyUIComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/Enemy/TwoMinWidgetEnemy.h"

ATwoMinEnemyCharacter::ATwoMinEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = CharacterRotationRate;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f; // 감속 속도

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidgetComponent");
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
	
	CharacterType = ECharacterType::Enemy;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
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
	
	if (UTwoMinWidgetEnemy* HealthWidget = Cast<UTwoMinWidgetEnemy>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyIComponent(EnemyUIComponent);
	}
}
