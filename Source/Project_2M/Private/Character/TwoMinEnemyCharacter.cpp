// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TwoMinEnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "TwoMinDebugHelper.h"
#include "TwoMinFunctionLibrary.h"
#include "TwoMinGameplayTag.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Blueprint/UserWidget.h"
#include "Compnents/ItemDropComponent.h"
#include "Compnents/Combat/EnemyCombatComponent.h"
#include "Compnents/UI/EnemyUIComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameInstance/TwoMinGameInstance.h"
#include "GameModes/TwoMinBaseGameMode.h"
#include "Item/EnterEvent/TwoMinEnterEventBase.h"
#include "Item/PickUp/TwoMinPickUpItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/SoundManager.h"
#include "Managers/WorldStageManager.h"
#include "Spawner/SpawnMonsterPointGroup.h"
#include "Widgets/Enemy/TwoMinWidgetEnemy.h"

ATwoMinEnemyCharacter::ATwoMinEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

	UpdateHealthWidgetPosition();
	UpdateExecutionWidgetPosition();
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
	
	if (bIsCinematic) return;

	AAIController* AI = Cast<AAIController>(GetController());
	if (!AI) return;

	AI->StopMovement();
	AI->BrainComponent->StopLogic(TEXT("AI Disabled"));
	AI->UnPossess();
	
	ATwoMinBaseGameMode* GM = Cast<ATwoMinBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return;
	
	if (IsUseBossHealthBar() == false)
	{
		GM->GetSpawnMonsterPointGroup()->AddDeathMonsterCount();
	}
	else
	{
		if (BossPhase == EBossPhaseType::Phase_Finish)
		{
			UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetGameInstance());
			if (!GI) return;
			
			GI->SoundManager->StopBGMSound();
			GM->GetSpawnMonsterPointGroup()->OpenBossStage();	
		}
		else
		{
			int32 PhaseValue = static_cast<int>(BossPhase);
			EBossPhaseType NextPhase = static_cast<EBossPhaseType>(PhaseValue - 1);
			
			FTimerManager& TimerManager = GetWorldTimerManager();
			if (TimerManager.IsTimerActive(PhaseConversionTimerHandle))
			{
				TimerManager.ClearTimer(PhaseConversionTimerHandle);
			}
			
			TimerManager.SetTimer(PhaseConversionTimerHandle,
				[this, NextPhase]()
				{
					GetEnemyUIComponent()->HideBossHealthBar();
					InitPhaseConversion(NextPhase);
				}, PhaseConversionDelay, false);
		}
	}
}

void ATwoMinEnemyCharacter::AfterDeathProcess()
{
	Super::AfterDeathProcess();
	
	if (bIsCinematic) return;
	if (IsUseBossHealthBar()) return;
	
	TMap<int32, int32> DropItems = ItemDropComponent->TryGetCharacterDropItems();
	
	if (DropItems.IsEmpty()) return;
	
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

void ATwoMinEnemyCharacter::StartDissolveProcess()
{
	Super::StartDissolveProcess();
	
	for (int32 Index = 0; Index < GetMesh()->GetNumMaterials(); Index++)
	{
		UMaterialInterface* Mat = GetMesh()->GetMaterial(Index);
		UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Mat, this);
    
		GetMesh()->SetMaterial(Index, DynMat);
		CachedDynamicMaterials.Add(DynMat);
	}
	
	GetWorldTimerManager().SetTimer(DissolveTimerHandle, this, &ATwoMinEnemyCharacter::UpdateDissolveMaterial,
		UpdateDissolveTime, true);
}

void ATwoMinEnemyCharacter::UpdateDissolveMaterial()
{
	CurDissolve += UpdateDissolveValue;
	for (auto DynamicMaterialInstance : CachedDynamicMaterials)
	{
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("DissolveAmount"), CurDissolve);
	}

	if (CurDissolve >= 1)
	{
		GetWorldTimerManager().ClearTimer(DissolveTimerHandle);
	}
}

void ATwoMinEnemyCharacter::OnDestroyedProcess()
{
	Super::OnDestroyedProcess();
	
	if (bIsCinematic) return;
	if (IsUseBossHealthBar() == false) return;
	if (BossPhase != EBossPhaseType::Phase_Finish) return;
	
	ClearStageProcess();
}

void ATwoMinEnemyCharacter::ClearStageProcess() const
{
	ATwoMinBaseGameMode* GM = Cast<ATwoMinBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return;
	
	UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetWorld()->GetGameInstance());
	if (!GI) return;
	
	GM->ShowClearStageUI();
	GI->StageManager->ClearCurrentWorldStage();
}

void ATwoMinEnemyCharacter::OnShowCharacter()
{
	if (bIsHideCharacter == false) return;
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	GetCombatComponent()->EnableWeaponsMesh(true);
	
	AAIController* AI = Cast<AAIController>(GetController());
	if (!AI) return;
	
	AI->BrainComponent->RestartLogic();
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AI->BrainComponent);
	AI->RunBehaviorTree(BTComp->GetCurrentTree());
	bIsHideCharacter = false;
	
	if (bIsNeedChangeBossBGMSound)
	{
		UTwoMinGameInstance* GI = Cast<UTwoMinGameInstance>(GetWorld()->GetGameInstance());
		if (!GI) return;
		
		GI->SoundManager->PlayBGMSound(BossBGMSound);
		bIsNeedChangeBossBGMSound = false;
	}
}

void ATwoMinEnemyCharacter::OnHideCharacter()
{
	if (bIsHideCharacter) return;
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	GetCombatComponent()->EnableWeaponsMesh(false);
	
	AAIController* AI = Cast<AAIController>(GetController());
	if (!AI) return;
	
	AI->StopMovement();
	AI->BrainComponent->StopLogic(TEXT("AI Disabled"));
	bIsHideCharacter = true;
}

void ATwoMinEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (bIsCinematic) return;
	
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CurStartUpData = GetCharacterStartUpData();
	if (CurStartUpData.IsNull()) return;
	
	UDataAsset_StartUpDataBase* LoadedData = CurStartUpData.LoadSynchronous();
	if (!LoadedData) return;
	
	LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent, MonsterLevel);
}

void ATwoMinEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (bIsCinematic)
	{
		// 스폰 몬스터와 겹치지 방지.
		InitCheckCinematic();
	}
}

void ATwoMinEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitEnemyHealthWidget();
}

void ATwoMinEnemyCharacter::InitCheckCinematic()
{
	if (bIsCinematic == false)	return;
	
	OnHideCharacter();
}

void ATwoMinEnemyCharacter::InitEnemyHealthWidget()
{
	UTwoMinWidgetEnemy* HealthWidget = GetEnemyHealthWidget();
	if (!HealthWidget) return;
	
	if (bUseBossHealthBar)
	{
		HealthWidget->HideWorldHealthBar();
		UTwoMinFunctionLibrary::AddGameplayTagToActor(this, TwoMinGameplayTag::Shared_State_Invincible);
	}
	else
	{
		HealthWidget->InitEnemyUIComponent(EnemyUIComponent);
	}
}

UTwoMinWidgetEnemy* ATwoMinEnemyCharacter::GetEnemyHealthWidget() const
{
	UUserWidget* UserWidget = EnemyHealthWidgetComponent->GetUserWidgetObject();
	if (!UserWidget) return nullptr;
		
	UTwoMinWidgetEnemy* HealthWidget = Cast<UTwoMinWidgetEnemy>(UserWidget);
	return HealthWidget;
}

void ATwoMinEnemyCharacter::InitPhaseConversion(EBossPhaseType NewBossPhase)
{
	const FString CurRealStageName = GetWorld()->RemovePIEPrefix(GetWorld()->GetMapName());
	if (!PhaseTargetCharacter.Contains(CurRealStageName)) return;
	
	FPhaseConversionData PhaseData = PhaseTargetCharacter[CurRealStageName];
	
	const TSubclassOf<ATwoMinEnemyCharacter> ConversionClass = PhaseData.BossPhaseType[NewBossPhase].PhaseTargetCharacter;
	if (!ConversionClass) return;
	
	ATwoMinEnemyCharacter* SpawnNewPhaseCharacter = 
		GetWorld()->SpawnActorDeferred<ATwoMinEnemyCharacter>(
			ConversionClass, 
			GetActorTransform(), 
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
			);
	
	SpawnNewPhaseCharacter->SetCharacterCinematicData(HideCinematicMap);
	SpawnNewPhaseCharacter->SetUseBossHealthBar(bUseBossHealthBar);
	SpawnNewPhaseCharacter->SetBossPhaseType(NewBossPhase);
	 
	SpawnNewPhaseCharacter->bIsNeedChangeBossBGMSound = true;
	SpawnNewPhaseCharacter->FinishSpawning(GetActorTransform());
	
	if (const TSubclassOf<ATwoMinEnterEventBase> PhaseLevelSequence = PhaseData.BossPhaseType[NewBossPhase].PhaseLevelSequence)
	{
		ATwoMinEnterEventBase* LevelSequence = GetWorld()->SpawnActorDeferred<ATwoMinEnterEventBase>(
		PhaseLevelSequence,
		GetActorTransform(),
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	
		LevelSequence->SetIgnoreCollision(true);
		LevelSequence->FinishSpawning(GetActorTransform());
	
		AActor* PlayerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
		LevelSequence->OutPlayLevelSequence(PlayerActor);
	}
	
	OnHideCharacter();
}

TSoftObjectPtr<UDataAsset_StartUpDataBase> ATwoMinEnemyCharacter::GetCharacterStartUpData() const
{
	return bUseBossHealthBar ? BossCharacterStartUpData : CharacterStartUpData;
}

bool ATwoMinEnemyCharacter::GetHideCinematic(const FString& PlayLevelSequenceName) const
{
	if (HideCinematicMap.Contains(PlayLevelSequenceName) == false) return false;
		
	return HideCinematicMap[PlayLevelSequenceName].bIsHide;
}

FString ATwoMinEnemyCharacter::GetSyncCinematicActorName(const FString& PlayLevelSequenceName) const
{
	if (HideCinematicMap.Contains(PlayLevelSequenceName) == false) return "";
		
	return HideCinematicMap[PlayLevelSequenceName].SyncCharacterName;
}

void ATwoMinEnemyCharacter::BossDetectProcess()
{
	UTwoMinGameInstance* GI =  Cast<UTwoMinGameInstance>(GetWorld()->GetGameInstance());
	if (!GI) return;
	
	GetEnemyUIComponent()->ShowBossHealthBar(GetMonsterName());
	GI->SoundManager->PlayBGMSound(BossBGMSound);
	UTwoMinFunctionLibrary::RemoveGameplayTagToActor(this, TwoMinGameplayTag::Shared_State_Invincible);
}


void ATwoMinEnemyCharacter::UpdateHealthWidgetPosition()
{
	if (!EnemyHealthWidgetComponent || EnemyHealthWidgetComponent->IsVisible() == false) return;
	
	FVector PlayerCameraPos = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FRotator Rotation = (PlayerCameraPos - GetActorLocation()).Rotation();
	
	EnemyHealthWidgetComponent->SetWorldRotation(Rotation);
}

void ATwoMinEnemyCharacter::EnableExecutionWidget(bool bIsEnable)
{
	if (!PossibleExecutionWidget)
	{
		UUserWidget* UserWidget = EnemyExecutionWidgetComponent->GetUserWidgetObject();
			
		UTwoMinWidget_PossibleExecution* CachedWidget = Cast<UTwoMinWidget_PossibleExecution>(UserWidget);
		if (!CachedWidget) return;
			
		PossibleExecutionWidget = CachedWidget;	
	}
		
	EnemyExecutionWidgetComponent->SetVisibility(bIsEnable);
	
	if (PossibleExecutionWidget)
	{
		PossibleExecutionWidget->ShowPossibleExecution();
	}
}

void ATwoMinEnemyCharacter::UpdateExecutionWidgetPosition()
{
	if (!EnemyExecutionWidgetComponent || EnemyExecutionWidgetComponent->IsVisible() == false) return;

	FVector Location = GetLockOnPos();
	FVector PlayerCameraPos = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FRotator Rotation = (PlayerCameraPos - GetActorLocation()).Rotation();
	
	EnemyExecutionWidgetComponent->SetWorldLocationAndRotation(Location, Rotation);
}
